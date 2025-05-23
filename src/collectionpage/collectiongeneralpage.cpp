/*
  SPDX-FileCopyrightText: 2009-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "collectiongeneralpage.h"
#include "collectiongeneralwidget.h"
#include <KStatefulBrush>
#include <PimCommonAkonadi/CollectionTypeUtil>

#include "folder/foldersettings.h"
#include "kernel/mailkernel.h"
#include <Akonadi/CollectionAnnotationsAttribute>
#include <PimCommonAkonadi/ContentTypeWidget>
#include <PimCommonAkonadi/IncidencesForWidget>

#include <Akonadi/AgentManager>
#include <Akonadi/AttributeFactory>
#include <Akonadi/Collection>
#include <Akonadi/CollectionModifyJob>
#include <Akonadi/EntityDisplayAttribute>
#include <PimCommon/PimUtil>

#include <KLineEditEventHandler>
#include <PimCommonAkonadi/ImapResourceCapabilitiesManager>

#include <KColorScheme>
#include <KLocalizedString>
#include <KMessageBox>

#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>

using namespace Akonadi;
using namespace MailCommon;

CollectionGeneralPage::CollectionGeneralPage(QWidget *parent)
    : CollectionPropertiesPage(parent)
{
    setObjectName(QLatin1StringView("MailCommon::CollectionGeneralPage"));
    setPageTitle(i18nc("@title:tab General settings for a folder.", "General"));
}

CollectionGeneralPage::~CollectionGeneralPage() = default;

void CollectionGeneralPage::init(const Akonadi::Collection &collection)
{
    mIsLocalSystemFolder = CommonKernel->isSystemFolderCollection(collection) || Kernel::folderIsInbox(collection);

    mIsResourceFolder = (collection.parentCollection() == Akonadi::Collection::root());
    auto topLayout = new QVBoxLayout(this);

    mCollectionGeneralWidget = new CollectionGeneralWidget(this);
    topLayout->addWidget(mCollectionGeneralWidget);
    auto innerLayout = qobject_cast<QFormLayout *>(mCollectionGeneralWidget->layout());

    // Mustn't be able to edit details for a non-resource, system folder.
    if ((!mIsLocalSystemFolder || mIsResourceFolder) && !mFolderCollection->isReadOnly()) {
        mNameEdit = new QLineEdit(this);
        KLineEditEventHandler::catchReturnKey(mNameEdit);
        connect(mNameEdit, &QLineEdit::textChanged, this, &CollectionGeneralPage::slotNameChanged);
        innerLayout->insertRow(0, i18nc("@label:textbox Name of the folder.", "Folder &Name:"), mNameEdit);
    }

    // Only do make this settable, if the IMAP resource is enabled
    // and it's not the personal folders (those must not be changed)
    const QString collectionResource = collection.resource();
    if (CommonKernel->imapResourceManager()->hasAnnotationSupport(collectionResource)) {
        PimCommon::CollectionTypeUtil::FolderContentsType contentsType = PimCommon::CollectionTypeUtil::ContentsTypeMail;

        const auto *annotationAttribute = collection.attribute<Akonadi::CollectionAnnotationsAttribute>();

        const QMap<QByteArray, QByteArray> annotations = (annotationAttribute ? annotationAttribute->annotations() : QMap<QByteArray, QByteArray>());

        const bool sharedSeen = (annotations.value(PimCommon::CollectionTypeUtil::kolabSharedSeen()) == "true");

        PimCommon::CollectionTypeUtil collectionUtil;
        const PimCommon::CollectionTypeUtil::IncidencesFor incidencesFor =
            collectionUtil.incidencesForFromString(QLatin1StringView(annotations.value(PimCommon::CollectionTypeUtil::kolabIncidencesFor())));

        const PimCommon::CollectionTypeUtil::FolderContentsType folderType =
            collectionUtil.typeFromKolabName(annotations.value(PimCommon::CollectionTypeUtil::kolabFolderType()));

        mContentsComboBox = new PimCommon::ContentTypeWidget(this);
        innerLayout->addRow(PimCommon::ContentTypeWidget::labelName(), mContentsComboBox);
        mContentsComboBox->setCurrentIndex(contentsType);

        connect(mContentsComboBox, &PimCommon::ContentTypeWidget::activated, this, &CollectionGeneralPage::slotFolderContentsSelectionChanged);

        if (mFolderCollection->isReadOnly() || mIsResourceFolder) {
            mContentsComboBox->setEnabled(false);
        }

        // Kolab incidences-for annotation.
        // Show incidences-for combobox if the contents type can be changed (new folder),
        // or if it's set to calendar or task (existing folder)
        const bool folderTypeComboboxEnabled =
            (folderType == PimCommon::CollectionTypeUtil::ContentsTypeCalendar || folderType == PimCommon::CollectionTypeUtil::ContentsTypeTask);
        if (folderTypeComboboxEnabled) {
            mIncidencesForComboBox = new PimCommon::IncidencesForWidget(this);
            innerLayout->addRow(PimCommon::IncidencesForWidget::labelName(), mIncidencesForComboBox);

            mIncidencesForComboBox->setCurrentIndex(incidencesFor);
        }

        mSharedSeenFlagsCheckBox = new QCheckBox(this);
        mSharedSeenFlagsCheckBox->setText(i18n("Share unread state with all users"));
        mSharedSeenFlagsCheckBox->setChecked(sharedSeen);
        mSharedSeenFlagsCheckBox->setWhatsThis(
            i18n("If enabled, the unread state of messages in this folder will be "
                 "the same for all users having access to this folder. If disabled "
                 "(the default), every user with access to this folder has their "
                 "own unread state."));
        innerLayout->addRow(QString(), mSharedSeenFlagsCheckBox);
    }

    topLayout->addStretch(100); // eat all superfluous space
}

void CollectionGeneralPage::load(const Akonadi::Collection &collection)
{
    mFolderCollection = FolderSettings::forCollection(collection);
    init(collection);

    mCollectionGeneralWidget->load(collection);
    if (mNameEdit) {
        const QString displayName = collection.displayName();

        if (!mIsLocalSystemFolder || mIsResourceFolder) {
            mNameEdit->setText(displayName);
        }
    }

    if (mContentsComboBox) {
        const auto *annotationsAttribute = collection.attribute<Akonadi::CollectionAnnotationsAttribute>();

        if (annotationsAttribute) {
            const QMap<QByteArray, QByteArray> annotations = annotationsAttribute->annotations();
            if (annotations.contains(PimCommon::CollectionTypeUtil::kolabFolderType())) {
                PimCommon::CollectionTypeUtil collectionUtil;
                mContentsComboBox->setCurrentItem(collectionUtil.typeNameFromKolabType(annotations[PimCommon::CollectionTypeUtil::kolabFolderType()]));
            }
        }
    }
}

void CollectionGeneralPage::save(Collection &collection)
{
    if (mNameEdit) {
        if (!mIsLocalSystemFolder) {
            const QString nameFolder(mNameEdit->text().trimmed());
            bool canRenameFolder = !(nameFolder.startsWith(QLatin1Char('.')) || nameFolder.endsWith(QLatin1Char('.')) || nameFolder.contains(QLatin1Char('/'))
                                     || nameFolder.isEmpty());

            if (mIsResourceFolder && (PimCommon::Util::isImapResource(collection.resource()))) {
                collection.setName(nameFolder);
                Akonadi::AgentInstance instance = Akonadi::AgentManager::self()->instance(collection.resource());
                instance.setName(nameFolder);
            } else if (canRenameFolder) {
                if (collection.hasAttribute<Akonadi::EntityDisplayAttribute>()
                    && !collection.attribute<Akonadi::EntityDisplayAttribute>()->displayName().isEmpty()) {
                    collection.attribute<Akonadi::EntityDisplayAttribute>()->setDisplayName(nameFolder);
                } else if (!nameFolder.isEmpty()) {
                    collection.setName(nameFolder);
                }
            }
        }
    }
    mCollectionGeneralWidget->save(collection);

    auto *annotationsAttribute = collection.attribute<Akonadi::CollectionAnnotationsAttribute>(Collection::AddIfMissing);

    QMap<QByteArray, QByteArray> annotations = annotationsAttribute->annotations();
    if (mSharedSeenFlagsCheckBox && mSharedSeenFlagsCheckBox->isEnabled()) {
        annotations[PimCommon::CollectionTypeUtil::kolabSharedSeen()] = mSharedSeenFlagsCheckBox->isChecked() ? "true" : "false";
    }

    PimCommon::CollectionTypeUtil collectionUtil;
    if (mIncidencesForComboBox && mIncidencesForComboBox->isEnabled()) {
        annotations[PimCommon::CollectionTypeUtil::kolabIncidencesFor()] =
            collectionUtil.incidencesForToString(static_cast<PimCommon::CollectionTypeUtil::IncidencesFor>(mIncidencesForComboBox->currentIndex())).toLatin1();
    }

    if (mContentsComboBox) {
        const PimCommon::CollectionTypeUtil::FolderContentsType type = collectionUtil.contentsTypeFromString(mContentsComboBox->currentText());

        const QByteArray kolabName = collectionUtil.kolabNameFromType(type);
        if (!kolabName.isEmpty()) {
            const QString iconName = collectionUtil.iconNameFromContentsType(type);
            auto *attribute = collection.attribute<Akonadi::EntityDisplayAttribute>(Akonadi::Collection::AddIfMissing);
            attribute->setIconName(iconName);
            new Akonadi::CollectionModifyJob(collection);
            annotations[PimCommon::CollectionTypeUtil::kolabFolderType()] = kolabName;
        }
    }
    if (annotations.isEmpty()) {
        collection.removeAttribute<Akonadi::CollectionAnnotationsAttribute>();
    } else {
        annotationsAttribute->setAnnotations(annotations);
    }
}

void CollectionGeneralPage::slotFolderContentsSelectionChanged(int)
{
    PimCommon::CollectionTypeUtil collectionUtil;
    const PimCommon::CollectionTypeUtil::FolderContentsType type = collectionUtil.contentsTypeFromString(mContentsComboBox->currentText());

    if (type != PimCommon::CollectionTypeUtil::ContentsTypeMail) {
        const QString message = i18n(
            "You have configured this folder to contain groupware information. "
            "That means that this folder will disappear once the configuration "
            "dialog is closed.");

        KMessageBox::information(this, message);
    }

    const bool enable = (type == PimCommon::CollectionTypeUtil::ContentsTypeCalendar || type == PimCommon::CollectionTypeUtil::ContentsTypeTask);

    if (mIncidencesForComboBox) {
        mIncidencesForComboBox->setEnabled(enable);
    }
}

void CollectionGeneralPage::slotNameChanged(const QString &name)
{
#ifndef QT_NO_STYLE_STYLESHEET
    QString styleSheet;
    if (name.startsWith(QLatin1Char('.')) || name.endsWith(QLatin1Char('.')) || name.contains(QLatin1Char('/')) || name.trimmed().isEmpty()) {
        if (mColorName.isEmpty()) {
            const KColorScheme::BackgroundRole bgColorScheme(KColorScheme::NegativeBackground);
            KStatefulBrush bgBrush(KColorScheme::View, bgColorScheme);
            mColorName = bgBrush.brush(palette()).color().name();
        }
        styleSheet = QStringLiteral("QLineEdit{ background-color:%1 }").arg(mColorName);
    }
    setStyleSheet(styleSheet);
#endif
}

#include "moc_collectiongeneralpage.cpp"
