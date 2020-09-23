/*
  SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "collectionexpirywidget.h"
#include "collectionexpiryjob.h"
#include "kernel/mailkernel.h"
#include "folderrequester.h"
#include "attributes/expirecollectionattribute.h"
#include "folder/foldersettings.h"
#include "util/mailutil.h"
#include "mailcommon_debug.h"

#include <CollectionModifyJob>

#include <KPluralHandlingSpinBox>
#include <KMessageBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>

using namespace MailCommon;

CollectionExpiryWidget::CollectionExpiryWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *globalVBox = new QVBoxLayout(this);

    QGridLayout *daysBox = new QGridLayout;

    expireReadMailCB = new QCheckBox(this);
    expireReadMailCB->setText(i18n("Expire read messages after"));
    connect(expireReadMailCB, &QCheckBox::toggled, this, &CollectionExpiryWidget::slotUpdateControls);
    daysBox->addWidget(expireReadMailCB, 0, 0);

    expireReadMailSB = new KPluralHandlingSpinBox(this);
    expireReadMailSB->setMaximum(999999);
    expireReadMailSB->setValue(30);
    expireReadMailSB->setSuffix(ki18ncp("Expire messages after %1", " day", " days"));
    daysBox->addWidget(expireReadMailSB, 0, 1);
    connect(expireReadMailSB, QOverload<int>::of(&KPluralHandlingSpinBox::valueChanged), this, &CollectionExpiryWidget::slotChanged);

    expireUnreadMailCB = new QCheckBox(this);
    expireUnreadMailCB->setText(i18n("Expire unread messages after"));
    connect(expireUnreadMailCB, &QCheckBox::toggled, this, &CollectionExpiryWidget::slotUpdateControls);
    daysBox->addWidget(expireUnreadMailCB, 1, 0);

    expireUnreadMailSB = new KPluralHandlingSpinBox(this);
    expireUnreadMailSB->setMaximum(99999);
    expireUnreadMailSB->setValue(30);
    expireUnreadMailSB->setSuffix(ki18ncp("Expire messages after %1", " day", " days"));
    daysBox->addWidget(expireUnreadMailSB, 1, 1);
    connect(expireUnreadMailSB, QOverload<int>::of(&KPluralHandlingSpinBox::valueChanged), this, &CollectionExpiryWidget::slotChanged);

    daysBox->setColumnStretch(3, 1);
    globalVBox->addLayout(daysBox);

    globalVBox->addSpacing(30);

    QGroupBox *actionsGroup = new QGroupBox(this);
    actionsGroup->setFlat(true); // for mutual exclusion of the radio buttons

    QHBoxLayout *moveToHBox = new QHBoxLayout();
    moveToHBox->setContentsMargins(0, 0, 0, 0);
    moveToHBox->setSpacing(6);

    moveToRB = new QRadioButton(actionsGroup);
    moveToRB->setText(i18n("Move expired messages to:"));
    connect(moveToRB, &QRadioButton::toggled, this, &CollectionExpiryWidget::slotUpdateControls);
    moveToHBox->addWidget(moveToRB);

    folderSelector = new FolderRequester(this);
    folderSelector->setMustBeReadWrite(true);
    folderSelector->setShowOutbox(false);
    moveToHBox->addWidget(folderSelector);
    globalVBox->addLayout(moveToHBox);
    connect(folderSelector, &FolderRequester::folderChanged, this, &CollectionExpiryWidget::slotChanged);

    deletePermanentlyRB = new QRadioButton(actionsGroup);
    deletePermanentlyRB->setText(i18n("Delete expired messages permanently"));
    connect(deletePermanentlyRB, &QRadioButton::toggled, this, &CollectionExpiryWidget::slotUpdateControls);

    globalVBox->addWidget(deletePermanentlyRB);

    globalVBox->addSpacing(30);

    expireNowPB = new QPushButton(i18n("Save Settings and Expire Now"), this);
    connect(expireNowPB, &QPushButton::clicked, this, &CollectionExpiryWidget::saveAndExpireRequested);
    globalVBox->addWidget(expireNowPB, 0, Qt::AlignRight);

    globalVBox->addStretch(100);   // eat all superfluous space
    deletePermanentlyRB->setChecked(true);
    slotUpdateControls();
}

CollectionExpiryWidget::~CollectionExpiryWidget()
{
}

void CollectionExpiryWidget::slotChanged()
{
    Q_EMIT configChanged();
}

void CollectionExpiryWidget::slotUpdateControls()
{
    const bool showExpiryActions = expireReadMailCB->isChecked() || expireUnreadMailCB->isChecked();
    moveToRB->setEnabled(showExpiryActions);
    folderSelector->setEnabled(showExpiryActions && moveToRB->isChecked());
    deletePermanentlyRB->setEnabled(showExpiryActions);

    expireReadMailSB->setEnabled(expireReadMailCB->isChecked());
    expireUnreadMailSB->setEnabled(expireUnreadMailCB->isChecked());

    expireNowPB->setEnabled(showExpiryActions);

    Q_EMIT configChanged();
}

void CollectionExpiryWidget::load(const MailCommon::CollectionExpirySettings &settings)
{
    if (settings.isValid()) {
        bool expiryGloballyOn = settings.expiryGloballyOn;
        if (expiryGloballyOn
            && settings.mReadExpireUnits != ExpireCollectionAttribute::ExpireNever
            && settings.daysToExpireRead >= 0) {
            expireReadMailCB->setChecked(true);
            expireReadMailSB->setValue(settings.daysToExpireRead);
        }
        if (expiryGloballyOn
            && settings.mUnreadExpireUnits != ExpireCollectionAttribute::ExpireNever
            && settings.daysToExpireUnread >= 0) {
            expireUnreadMailCB->setChecked(true);
            expireUnreadMailSB->setValue(settings.daysToExpireUnread);
        }

        if (settings.mExpireAction == ExpireCollectionAttribute::ExpireDelete) {
            deletePermanentlyRB->setChecked(true);
        } else {
            moveToRB->setChecked(true);
        }

        Akonadi::Collection::Id destFolderID = settings.mExpireToFolderId;
        if (destFolderID > 0) {
            Akonadi::Collection destFolder = Kernel::self()->collectionFromId(destFolderID);
            if (destFolder.isValid()) {
                folderSelector->setCollection(destFolder);
            }
        }
    } else {
        deletePermanentlyRB->setChecked(true);
    }
    slotUpdateControls();
}

bool CollectionExpiryWidget::validateExpireFolder(bool expireNow)
{
    const bool enableGlobally = expireReadMailCB->isChecked() || expireUnreadMailCB->isChecked();
    const Akonadi::Collection expireToFolder = folderSelector->collection();
    if (enableGlobally && moveToRB->isChecked() && !expireToFolder.isValid()) {
        KMessageBox::error(this, i18n("Please select a folder to expire messages into.\nIf this is not done, expired messages will be permanently deleted."),
                           i18n("No Folder Selected"));
        deletePermanentlyRB->setChecked(true);
        expireNow = false;                                // settings are not valid
    }
    return expireNow;
}

MailCommon::ExpireCollectionAttribute *CollectionExpiryWidget::assignFolderAttribute(Akonadi::Collection &collection, bool &expireNow)
{
    const Akonadi::Collection expireToFolder = folderSelector->collection();
    MailCommon::ExpireCollectionAttribute *attribute = nullptr;
    if (expireToFolder.isValid() && moveToRB->isChecked()) {
        if (expireToFolder.id() == collection.id()) {
            KMessageBox::error(this, i18n("Please select a different folder than the current folder to expire messages into.\nIf this is not done, expired messages will be permanently deleted."),
                               i18n("Wrong Folder Selected"));
            deletePermanentlyRB->setChecked(true);
            expireNow = false;                                // settings are not valid
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
    settings.expiryGloballyOn = expireReadMailCB->isChecked() || expireUnreadMailCB->isChecked();
    // we always write out days now
    settings.daysToExpireRead = expireReadMailSB->value();
    settings.daysToExpireUnread = expireUnreadMailSB->value();
    settings.mReadExpireUnits = expireReadMailCB->isChecked() ? MailCommon::ExpireCollectionAttribute::ExpireDays : MailCommon::ExpireCollectionAttribute::ExpireNever;
    settings.mUnreadExpireUnits = expireUnreadMailCB->isChecked() ? MailCommon::ExpireCollectionAttribute::ExpireDays : MailCommon::ExpireCollectionAttribute::ExpireNever;

    if (deletePermanentlyRB->isChecked()) {
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
        CollectionExpiryJob *job = new CollectionExpiryJob;
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
