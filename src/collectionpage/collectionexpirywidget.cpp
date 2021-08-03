/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "collectionexpirywidget.h"
#include "attributes/expirecollectionattribute.h"
#include "collectionexpiryjob.h"
#include "folder/foldersettings.h"
#include "folderrequester.h"
#include "kernel/mailkernel.h"
#include "mailcommon_debug.h"
#include "util/mailutil.h"

#include <CollectionModifyJob>

#include <KMessageBox>
#include <KPluralHandlingSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

using namespace MailCommon;

CollectionExpiryWidget::CollectionExpiryWidget(QWidget *parent)
    : QWidget(parent)
{
    auto globalVBox = new QVBoxLayout(this);
    globalVBox->setContentsMargins({});

    auto daysBox = new QGridLayout;

    mExpireReadMailCB = new QCheckBox(i18n("Expire read messages after"), this);
    connect(mExpireReadMailCB, &QCheckBox::toggled, this, &CollectionExpiryWidget::slotUpdateControls);
    daysBox->addWidget(mExpireReadMailCB, 0, 0);

    mExpireReadMailSB = new KPluralHandlingSpinBox(this);
    mExpireReadMailSB->setMaximum(999999);
    mExpireReadMailSB->setValue(30);
    mExpireReadMailSB->setSuffix(ki18ncp("Expire messages after %1", " day", " days"));
    daysBox->addWidget(mExpireReadMailSB, 0, 1);
    connect(mExpireReadMailSB, QOverload<int>::of(&KPluralHandlingSpinBox::valueChanged), this, &CollectionExpiryWidget::slotChanged);

    mExpireUnreadMailCB = new QCheckBox(i18n("Expire unread messages after"), this);
    connect(mExpireUnreadMailCB, &QCheckBox::toggled, this, &CollectionExpiryWidget::slotUpdateControls);
    daysBox->addWidget(mExpireUnreadMailCB, 1, 0);

    mExpireUnreadMailSB = new KPluralHandlingSpinBox(this);
    mExpireUnreadMailSB->setMaximum(99999);
    mExpireUnreadMailSB->setValue(30);
    mExpireUnreadMailSB->setSuffix(ki18ncp("Expire messages after %1", " day", " days"));
    daysBox->addWidget(mExpireUnreadMailSB, 1, 1);
    connect(mExpireUnreadMailSB, QOverload<int>::of(&KPluralHandlingSpinBox::valueChanged), this, &CollectionExpiryWidget::slotChanged);

    mExpireMailWithInvalidDateCB = new QCheckBox(i18n("Expire messages with invalid date"), this);
    connect(mExpireMailWithInvalidDateCB, &QCheckBox::toggled, this, &CollectionExpiryWidget::slotChanged);
    daysBox->addWidget(mExpireMailWithInvalidDateCB, 2, 0);

    daysBox->setColumnStretch(3, 1);
    globalVBox->addLayout(daysBox);

    globalVBox->addSpacing(30);

    auto actionsGroup = new QGroupBox(this);
    actionsGroup->setFlat(true); // for mutual exclusion of the radio buttons
    globalVBox->addWidget(actionsGroup);

    auto moveToHBox = new QHBoxLayout();
    moveToHBox->setContentsMargins(0, 0, 0, 0);
    moveToHBox->setSpacing(6);

    mMoveToRB = new QRadioButton(actionsGroup);
    mMoveToRB->setText(i18n("Move expired messages to:"));
    connect(mMoveToRB, &QRadioButton::toggled, this, &CollectionExpiryWidget::slotUpdateControls);
    moveToHBox->addWidget(mMoveToRB);

    mFolderSelector = new FolderRequester(this);
    mFolderSelector->setMustBeReadWrite(true);
    mFolderSelector->setShowOutbox(false);
    moveToHBox->addWidget(mFolderSelector);
    globalVBox->addLayout(moveToHBox);
    connect(mFolderSelector, &FolderRequester::folderChanged, this, &CollectionExpiryWidget::slotChanged);

    mDeletePermanentlyRB = new QRadioButton(actionsGroup);
    mDeletePermanentlyRB->setText(i18n("Delete expired messages permanently"));
    connect(mDeletePermanentlyRB, &QRadioButton::toggled, this, &CollectionExpiryWidget::slotUpdateControls);

    globalVBox->addWidget(mDeletePermanentlyRB);

    globalVBox->addSpacing(30);

    mExpireNowPB = new QPushButton(i18n("Save Settings and Expire Now"), this);
    connect(mExpireNowPB, &QPushButton::clicked, this, &CollectionExpiryWidget::saveAndExpireRequested);
    globalVBox->addWidget(mExpireNowPB, 0, Qt::AlignRight);

    globalVBox->addStretch(100); // eat all superfluous space
    mDeletePermanentlyRB->setChecked(true);
    slotUpdateControls();
}

CollectionExpiryWidget::~CollectionExpiryWidget()
{
}

void CollectionExpiryWidget::hideExpireNowButton()
{
    mExpireNowPB->setVisible(false);
}

void CollectionExpiryWidget::slotChanged()
{
    Q_EMIT configChanged();
}

void CollectionExpiryWidget::slotUpdateControls()
{
    const bool showExpiryActions = mExpireReadMailCB->isChecked() || mExpireUnreadMailCB->isChecked();
    mMoveToRB->setEnabled(showExpiryActions);
    mFolderSelector->setEnabled(showExpiryActions && mMoveToRB->isChecked());
    mDeletePermanentlyRB->setEnabled(showExpiryActions);

    mExpireReadMailSB->setEnabled(mExpireReadMailCB->isChecked());
    mExpireUnreadMailSB->setEnabled(mExpireUnreadMailCB->isChecked());

    mExpireNowPB->setEnabled(showExpiryActions);

    Q_EMIT configChanged();
}

void CollectionExpiryWidget::load(const MailCommon::CollectionExpirySettings &settings)
{
    if (settings.isValid()) {
        bool expiryGloballyOn = settings.expiryGloballyOn;
        if (expiryGloballyOn && settings.mReadExpireUnits != ExpireCollectionAttribute::ExpireNever && settings.daysToExpireRead >= 0) {
            mExpireReadMailCB->setChecked(true);
            mExpireReadMailSB->setValue(settings.daysToExpireRead);
        }
        if (expiryGloballyOn && settings.mUnreadExpireUnits != ExpireCollectionAttribute::ExpireNever && settings.daysToExpireUnread >= 0) {
            mExpireUnreadMailCB->setChecked(true);
            mExpireUnreadMailSB->setValue(settings.daysToExpireUnread);
        }

        if (settings.mExpireAction == ExpireCollectionAttribute::ExpireDelete) {
            mDeletePermanentlyRB->setChecked(true);
        } else {
            mMoveToRB->setChecked(true);
        }

        mExpireMailWithInvalidDateCB->setChecked(settings.expiryMessagesWithInvalidDate);
        Akonadi::Collection::Id destFolderID = settings.mExpireToFolderId;
        if (destFolderID > 0) {
            Akonadi::Collection destFolder = Kernel::self()->collectionFromId(destFolderID);
            if (destFolder.isValid()) {
                mFolderSelector->setCollection(destFolder);
            }
        }
    } else {
        mDeletePermanentlyRB->setChecked(true);
    }
    slotUpdateControls();
}

bool CollectionExpiryWidget::validateExpireFolder(bool expireNow)
{
    const bool enableGlobally = mExpireReadMailCB->isChecked() || mExpireUnreadMailCB->isChecked();
    const Akonadi::Collection expireToFolder = mFolderSelector->collection();
    if (enableGlobally && mMoveToRB->isChecked() && !expireToFolder.isValid()) {
        KMessageBox::error(this,
                           i18n("Please select a folder to expire messages into.\nIf this is not done, expired messages will be permanently deleted."),
                           i18n("No Folder Selected"));
        mDeletePermanentlyRB->setChecked(true);
        expireNow = false; // settings are not valid
    }
    return expireNow;
}

MailCommon::ExpireCollectionAttribute *CollectionExpiryWidget::assignFolderAttribute(Akonadi::Collection &collection, bool &expireNow)
{
    const Akonadi::Collection expireToFolder = mFolderSelector->collection();
    MailCommon::ExpireCollectionAttribute *attribute = nullptr;
    if (expireToFolder.isValid() && mMoveToRB->isChecked()) {
        if (expireToFolder.id() == collection.id()) {
            KMessageBox::error(this,
                               i18n("Please select a different folder than the current folder to expire messages into.\nIf this is not done, expired messages "
                                    "will be permanently deleted."),
                               i18n("Wrong Folder Selected"));
            mDeletePermanentlyRB->setChecked(true);
            expireNow = false; // settings are not valid
        } else {
            attribute = collection.attribute<MailCommon::ExpireCollectionAttribute>(Akonadi::Collection::AddIfMissing);
            attribute->setExpireToFolderId(expireToFolder.id());
        }
    }
    if (!attribute) {
        attribute = collection.attribute<MailCommon::ExpireCollectionAttribute>(Akonadi::Collection::AddIfMissing);
    }
    return attribute;
}

CollectionExpirySettings CollectionExpiryWidget::settings() const
{
    CollectionExpirySettings settings;
    settings.expiryGloballyOn = mExpireReadMailCB->isChecked() || mExpireUnreadMailCB->isChecked();
    settings.expiryMessagesWithInvalidDate = mExpireMailWithInvalidDateCB->isChecked();
    // we always write out days now
    settings.daysToExpireRead = mExpireReadMailSB->value();
    settings.daysToExpireUnread = mExpireUnreadMailSB->value();
    settings.mReadExpireUnits =
        mExpireReadMailCB->isChecked() ? MailCommon::ExpireCollectionAttribute::ExpireDays : MailCommon::ExpireCollectionAttribute::ExpireNever;
    settings.mUnreadExpireUnits =
        mExpireUnreadMailCB->isChecked() ? MailCommon::ExpireCollectionAttribute::ExpireDays : MailCommon::ExpireCollectionAttribute::ExpireNever;

    if (mDeletePermanentlyRB->isChecked()) {
        settings.mExpireAction = ExpireCollectionAttribute::ExpireDelete;
    } else {
        settings.mExpireAction = ExpireCollectionAttribute::ExpireMove;
    }
    return settings;
}

void CollectionExpiryWidget::save(const CollectionExpirySettings &collectionExpirySettings, Akonadi::Collection &collection, bool saveSettings, bool expireNow)
{
    expireNow = validateExpireFolder(expireNow);
    MailCommon::ExpireCollectionAttribute *attribute = assignFolderAttribute(collection, expireNow);
    attribute->setAutoExpire(collectionExpirySettings.expiryGloballyOn);
    // we always write out days now
    attribute->setReadExpireAge(collectionExpirySettings.daysToExpireRead);
    attribute->setUnreadExpireAge(collectionExpirySettings.daysToExpireUnread);
    attribute->setReadExpireUnits(collectionExpirySettings.mReadExpireUnits);
    attribute->setUnreadExpireUnits(collectionExpirySettings.mUnreadExpireUnits);
    attribute->setExpireAction(collectionExpirySettings.mExpireAction);

    if (saveSettings) {
        auto job = new CollectionExpiryJob;
        job->setExpireNow(expireNow);
        job->setCollection(collection);
        job->start();
    } else {
        if (expireNow) {
            MailCommon::Util::expireOldMessages(collection, true /*immediate*/);
        }
    }
    Q_EMIT configChanged(false);
}

void CollectionExpiryWidget::save(Akonadi::Collection &collection, bool saveSettings, bool expireNow)
{
    const CollectionExpirySettings collectionExpirySettings = settings();
    save(collectionExpirySettings, collection, saveSettings, expireNow);
}

bool CollectionExpiryWidget::canHandle(const Akonadi::Collection &col)
{
    QSharedPointer<FolderSettings> fd = FolderSettings::forCollection(col, false);
    return fd->canDeleteMessages() && !fd->isStructural() && !MailCommon::Util::isVirtualCollection(col);
}

bool CollectionExpirySettings::isValid() const
{
    const bool valid = daysToExpireRead != -1 || daysToExpireUnread != -1 || mUnreadExpireUnits != ExpireCollectionAttribute::ExpireNever
        || mReadExpireUnits != ExpireCollectionAttribute::ExpireNever || mExpireAction != ExpireCollectionAttribute::ExpireDelete || mExpireToFolderId != -1;
    return valid;
}

QDebug operator<<(QDebug d, const CollectionExpirySettings &t)
{
    d << " expiryGloballyOn " << t.expiryGloballyOn;
    d << " expiryMessagesWithInvalidDate " << t.expiryMessagesWithInvalidDate;
    d << " daysToExpireRead " << t.daysToExpireRead;
    d << " daysToExpireUnread " << t.daysToExpireUnread;
    d << " mUnreadExpireUnits " << t.mUnreadExpireUnits;
    d << " mReadExpireUnits " << t.mReadExpireUnits;
    d << " mExpireAction " << t.mExpireAction;
    d << " mExpireToFolderId " << t.mExpireToFolderId;
    return d;
}
