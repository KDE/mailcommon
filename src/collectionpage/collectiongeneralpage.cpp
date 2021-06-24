/*
  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "collectiongeneralpage.h"
#include "collectiongeneralwidget.h"
#include "collectiontypeutil.h"

#include "folder/foldersettings.h"
#include "incidencesforwidget.h"
#include "kernel/mailkernel.h"
#include "util/mailutil_p.h"
#include <PimCommonAkonadi/CollectionAnnotationsAttribute>
#include <PimCommonAkonadi/ContentTypeWidget>

#include <AgentManager>
#include <AttributeFactory>
#include <Collection>
#include <CollectionModifyJob>
#include <EntityDisplayAttribute>
#include <PimCommon/PimUtil>

#include <KIdentityManagement/IdentityCombo>
#include <Libkdepim/LineEditCatchReturnKey>
#include <PimCommonAkonadi/ImapResourceCapabilitiesManager>

#include <KColorScheme>
#include <KLocalizedString>
#include <KMessageBox>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

using namespace Akonadi;
using namespace MailCommon;

CollectionGeneralPage::CollectionGeneralPage(QWidget *parent)
    : CollectionPropertiesPage(parent)
{
    setObjectName(QStringLiteral("MailCommon::CollectionGeneralPage"));
    setPageTitle(i18nc("@title:tab General settings for a folder.", "General"));
}

CollectionGeneralPage::~CollectionGeneralPage()
{
}

void CollectionGeneralPage::init(const Akonadi::Collection &collection)
{
    mIsLocalSystemFolder = CommonKernel->isSystemFolderCollection(collection) || Kernel::folderIsInbox(collection);

    mIsResourceFolder = (collection.parentCollection() == Akonadi::Collection::root());
    QLabel *label = nullptr;

    auto topLayout = new QVBoxLayout(this);

    // Musn't be able to edit details for a non-resource, system folder.
    if ((!mIsLocalSystemFolder || mIsResourceFolder) && !mFolderCollection->isReadOnly()) {
        auto hl = new QHBoxLayout();
        topLayout->addItem(hl);
        label = new QLabel(i18nc("@label:textbox Name of the folder.", "&Name:"), this);
        hl->addWidget(label);

        mNameEdit = new QLineEdit(this);
        new KPIM::LineEditCatchReturnKey(mNameEdit, this);

        connect(mNameEdit, &QLineEdit::textChanged, this, &CollectionGeneralPage::slotNameChanged);
        label->setBuddy(mNameEdit);
        hl->addWidget(mNameEdit);
    }

    mCollectionGeneralWidget = new CollectionGeneralWidget(this);
    topLayout->addWidget(mCollectionGeneralWidget);
    // Only do make this settable, if the IMAP resource is enabled
    // and it's not the personal folders (those must not be changed)
    const QString collectionResource = collection.resource();
    if (CommonKernel->imapResourceManager()->hasAnnotationSupport(collectionResource)) {
        PimCommon::CollectionTypeUtil::FolderContentsType contentsType = PimCommon::CollectionTypeUtil::ContentsTypeMail;

        const auto *annotationAttribute = collection.attribute<PimCommon::CollectionAnnotationsAttribute>();

        const QMap<QByteArray, QByteArray> annotations = (annotationAttribute ? annotationAttribute->annotations() : QMap<QByteArray, QByteArray>());

        const bool sharedSeen = (annotations.value(PimCommon::CollectionTypeUtil::kolabSharedSeen()) == "true");

        PimCommon::CollectionTypeUtil collectionUtil;
        const PimCommon::CollectionTypeUtil::IncidencesFor incidencesFor =
            collectionUtil.incidencesForFromString(QLatin1String(annotations.value(PimCommon::CollectionTypeUtil::kolabIncidencesFor())));

        const PimCommon::CollectionTypeUtil::FolderContentsType folderType =
            collectionUtil.typeFromKolabName(annotations.value(PimCommon::CollectionTypeUtil::kolabFolderType()));

        int row = 0;
        auto gl = new QGridLayout();
        topLayout->addItem(gl);
        mContentsComboBox = new PimCommon::ContentTypeWidget(this);
        gl->addWidget(mContentsComboBox, row, 0, 1, 2);
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
        ++row;
        mIncidencesForComboBox = new PimCommon::IncidencesForWidget(this);
        gl->addWidget(mIncidencesForComboBox, row, 0, 1, 2);

        mIncidencesForComboBox->setCurrentIndex(incidencesFor);
        mIncidencesForComboBox->setEnabled(folderTypeComboboxEnabled);

        mSharedSeenFlagsCheckBox = new QCheckBox(this);
        mSharedSeenFlagsCheckBox->setText(i18n("Share unread state with all users"));
        mSharedSeenFlagsCheckBox->setChecked(sharedSeen);
        ++row;
        gl->addWidget(mSharedSeenFlagsCheckBox, row, 0, 1, 1);
        mSharedSeenFlagsCheckBox->setWhatsThis(
            i18n("If enabled, the unread state of messages in this folder will be "
                 "the same for all users having access to this folder. If disabled "
                 "(the default), every user with access to this folder has their "
                 "own unread state."));
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
        const auto *annotationsAttribute = collection.attribute<PimCommon::CollectionAnnotationsAttribute>();

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

    auto *annotationsAttribute = collection.attribute<PimCommon::CollectionAnnotationsAttribute>(Collection::AddIfMissing);

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
        collection.removeAttribute<PimCommon::CollectionAnnotationsAttribute>();
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
