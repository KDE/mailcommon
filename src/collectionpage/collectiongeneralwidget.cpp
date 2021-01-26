/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "collectiongeneralwidget.h"
#include "folder/foldersettings.h"
#include "kernel/mailkernel.h"
#include "util/mailutil_p.h"
#include <Akonadi/KMime/NewMailNotifierAttribute>
#include <KIdentityManagement/IdentityCombo>
#include <KLocalizedString>
#include <QCheckBox>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
using namespace MailCommon;

CollectionGeneralWidget::CollectionGeneralWidget(QWidget *parent)
    : QWidget(parent)
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));
    topLayout->setContentsMargins({});
    // should new mail in this folder be ignored?
    auto hbl = new QHBoxLayout();
    topLayout->addItem(hbl);
    mNotifyOnNewMailCheckBox = new QCheckBox(i18n("Act on new/unread mail in this folder"), this);
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
    hbl->addWidget(mNotifyOnNewMailCheckBox);
    // should replies to mails in this folder be kept in this same folder?
    hbl = new QHBoxLayout();
    topLayout->addItem(hbl);
    mKeepRepliesInSameFolderCheckBox = new QCheckBox(i18n("Keep replies in this folder"), this);
    mKeepRepliesInSameFolderCheckBox->setWhatsThis(
        i18n("Check this option if you want replies you write "
             "to mails in this folder to be put in this same folder "
             "after sending, instead of in the configured sent-mail folder."));
    hbl->addWidget(mKeepRepliesInSameFolderCheckBox);
    hbl->addStretch(1);
    // should this folder be shown in the folder selection dialog?
    hbl = new QHBoxLayout();
    topLayout->addItem(hbl);
    mHideInSelectionDialogCheckBox = new QCheckBox(i18n("Hide this folder in the folder selection dialog"), this);
    mHideInSelectionDialogCheckBox->setWhatsThis(xi18nc("@info:whatsthis",
                                                        "Check this option if you do not want this folder "
                                                        "to be shown in folder selection dialogs, such as the <interface>"
                                                        "Jump to Folder</interface> dialog."));
    hbl->addWidget(mHideInSelectionDialogCheckBox);
    hbl->addStretch(1);

    addLine(this, topLayout);
    // use grid layout for the following combobox settings
    auto gl = new QGridLayout();
    topLayout->addItem(gl);
    gl->setColumnStretch(1, 100); // make the second column use all available space
    int row = -1;

    // sender identity
    ++row;
    mUseDefaultIdentityCheckBox = new QCheckBox(i18n("Use &default identity"), this);
    gl->addWidget(mUseDefaultIdentityCheckBox);
    connect(mUseDefaultIdentityCheckBox, &QCheckBox::stateChanged, this, &CollectionGeneralWidget::slotIdentityCheckboxChanged);
    ++row;
    auto label = new QLabel(i18n("&Sender identity:"), this);
    gl->addWidget(label, row, 0);
    mIdentityComboBox = new KIdentityManagement::IdentityCombo(KernelIf->identityManager(), this);
    label->setBuddy(mIdentityComboBox);
    gl->addWidget(mIdentityComboBox, row, 1);
    mIdentityComboBox->setWhatsThis(
        i18n("Select the sender identity to be used when writing new mail "
             "or replying to mail in this folder. This means that if you are in "
             "one of your work folders, you can make KMail use the corresponding "
             "sender email address, signature and signing or encryption keys "
             "automatically. Identities can be set up in the main configuration "
             "dialog. (Settings -> Configure KMail)"));
}

CollectionGeneralWidget::~CollectionGeneralWidget()
{
}

void CollectionGeneralWidget::addLine(QWidget *parent, QVBoxLayout *layout)
{
    auto line = new QFrame(parent);
    line->setGeometry(QRect(80, 150, 250, 20));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setFrameShape(QFrame::HLine);
    layout->addWidget(line);
}

void CollectionGeneralWidget::slotIdentityCheckboxChanged()
{
    mIdentityComboBox->setEnabled(!mUseDefaultIdentityCheckBox->isChecked());
    if (mFolderCollection && mUseDefaultIdentityCheckBox->isChecked()) {
        mIdentityComboBox->setCurrentIdentity(mFolderCollection->fallBackIdentity());
    }
}

void CollectionGeneralWidget::save(Akonadi::Collection &collection)
{
    if (!mFolderCollection) {
        mFolderCollection = FolderSettings::forCollection(collection);
    }
    if (!mNotifyOnNewMailCheckBox->isChecked()) {
        auto *newMailNotifierAttr = collection.attribute<Akonadi::NewMailNotifierAttribute>(Akonadi::Collection::AddIfMissing);
        newMailNotifierAttr->setIgnoreNewMail(true);
    } else {
        collection.removeAttribute<Akonadi::NewMailNotifierAttribute>();
    }
    if (mFolderCollection) {
        mFolderCollection->setIdentity(mIdentityComboBox->currentIdentity());
        mFolderCollection->setUseDefaultIdentity(mUseDefaultIdentityCheckBox->isChecked());

        mFolderCollection->setPutRepliesInSameFolder(mKeepRepliesInSameFolderCheckBox->isChecked());
        mFolderCollection->setHideInSelectionDialog(mHideInSelectionDialogCheckBox->isChecked());
        mFolderCollection->writeConfig();
    }
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
