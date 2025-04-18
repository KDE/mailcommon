/*
  SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "collectiongeneralwidget.h"
#include "folder/foldersettings.h"
#include "kernel/mailkernel.h"
#include "util/mailutil_p.h"
#include <Akonadi/NewMailNotifierAttribute>
#include <KIdentityManagementWidgets/IdentityCombo>
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
using namespace MailCommon;

CollectionGeneralWidget::CollectionGeneralWidget(QWidget *parent)
    : QWidget(parent)
{
    auto topLayout = new QFormLayout(this);
    topLayout->setObjectName(QLatin1StringView("topLayout"));
    topLayout->setContentsMargins({});
    // should new mail in this folder be ignored?
    mNotifyOnNewMailCheckBox = new QCheckBox(i18nc("@option:check", "Act on new/unread mail in this folder"), this);
    mNotifyOnNewMailCheckBox->setWhatsThis(
        i18n("<qt><p>If this option is enabled then you will be notified about "
             "new/unread mail in this folder. Moreover, going to the "
             "next/previous folder with unread messages will stop at this "
             "folder.</p>"
             "<p>Uncheck this option if you do not want to be notified about "
             "new/unread mail in this folder and if you want this folder to "
             "be skipped when going to the next/previous folder with unread "
             "messages. This is useful for ignoring any new/unread mail in "
             "your trash and spam folder.</p></qt>"));
    topLayout->addRow(QString(), mNotifyOnNewMailCheckBox);
    // should replies to mails in this folder be kept in this same folder?
    mKeepRepliesInSameFolderCheckBox = new QCheckBox(i18nc("@option:check", "Keep replies in this folder"), this);
    mKeepRepliesInSameFolderCheckBox->setWhatsThis(
        i18n("Check this option if you want replies you write "
             "to mails in this folder to be put in this same folder "
             "after sending, instead of in the configured sent-mail folder."));
    topLayout->addRow(QString(), mKeepRepliesInSameFolderCheckBox);

    // should this folder be shown in the folder selection dialog?
    mHideInSelectionDialogCheckBox = new QCheckBox(i18nc("@option:check", "Hide this folder in the folder selection dialog"), this);
    mHideInSelectionDialogCheckBox->setWhatsThis(xi18nc("@info:whatsthis",
                                                        "Check this option if you do not want this folder "
                                                        "to be shown in folder selection dialogs, such as the <interface>"
                                                        "Jump to Folder</interface> dialog."));
    topLayout->addRow(QString(), mHideInSelectionDialogCheckBox);

    // sender identity
    mUseDefaultIdentityCheckBox = new QCheckBox(i18nc("@option:check", "Use &default identity"), this);
    topLayout->addRow(QString(), mUseDefaultIdentityCheckBox);
    connect(mUseDefaultIdentityCheckBox, &QCheckBox::checkStateChanged, this, &CollectionGeneralWidget::slotIdentityCheckboxChanged);

    mIdentityComboBox = new KIdentityManagementWidgets::IdentityCombo(KernelIf->identityManager(), this);
    mIdentityComboBox->setWhatsThis(
        i18n("Select the sender identity to be used when writing new mail "
             "or replying to mail in this folder. This means that if you are in "
             "one of your work folders, you can make KMail use the corresponding "
             "sender email address, signature and signing or encryption keys "
             "automatically. Identities can be set up in the main configuration "
             "dialog. (Settings -> Configure KMail)"));
    topLayout->addRow(i18n("&Sender identity:"), mIdentityComboBox);
}

CollectionGeneralWidget::~CollectionGeneralWidget() = default;

void CollectionGeneralWidget::slotIdentityCheckboxChanged()
{
    mIdentityComboBox->setEnabled(!mUseDefaultIdentityCheckBox->isChecked());
    if (!mFolderCollection.isNull() && mUseDefaultIdentityCheckBox->isChecked()) {
        mIdentityComboBox->setCurrentIdentity(mFolderCollection->fallBackIdentity());
    }
}

void CollectionGeneralWidget::save(Akonadi::Collection &collection)
{
    if (mFolderCollection.isNull()) {
        mFolderCollection = FolderSettings::forCollection(collection);
    }
    if (!mNotifyOnNewMailCheckBox->isChecked()) {
        auto *newMailNotifierAttr = collection.attribute<Akonadi::NewMailNotifierAttribute>(Akonadi::Collection::AddIfMissing);
        newMailNotifierAttr->setIgnoreNewMail(true);
    } else {
        collection.removeAttribute<Akonadi::NewMailNotifierAttribute>();
    }
    if (!mFolderCollection.isNull()) {
        mFolderCollection->setIdentity(mIdentityComboBox->currentIdentity());
        mFolderCollection->setUseDefaultIdentity(mUseDefaultIdentityCheckBox->isChecked());

        mFolderCollection->setPutRepliesInSameFolder(mKeepRepliesInSameFolderCheckBox->isChecked());
        mFolderCollection->setHideInSelectionDialog(mHideInSelectionDialogCheckBox->isChecked());
        mFolderCollection->writeConfig();
    }
    mFolderCollection.reset();
}

void CollectionGeneralWidget::load(const Akonadi::Collection &col)
{
    mFolderCollection = MailCommon::FolderSettings::forCollection(col);
    // folder identity
    mIdentityComboBox->setCurrentIdentity(mFolderCollection->identity());
    mUseDefaultIdentityCheckBox->setChecked(mFolderCollection->useDefaultIdentity());

    // ignore new mail
    mNotifyOnNewMailCheckBox->setChecked(!Util::ignoreNewMailInFolder(col));

    const bool keepInFolder = (mFolderCollection->canCreateMessages() && mFolderCollection->putRepliesInSameFolder());

    mKeepRepliesInSameFolderCheckBox->setChecked(keepInFolder);
    mKeepRepliesInSameFolderCheckBox->setEnabled(mFolderCollection->canCreateMessages());
    mHideInSelectionDialogCheckBox->setChecked(mFolderCollection->hideInSelectionDialog());
}

#include "moc_collectiongeneralwidget.cpp"
