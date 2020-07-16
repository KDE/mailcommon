/*
  Copyright (c) 2020 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "collectionexpirywidget.h"
#include "kernel/mailkernel.h"
#include "folderrequester.h"
#include <KPluralHandlingSpinBox>
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
    daysBox->addWidget(expireReadMailCB, 0, 0, Qt::AlignLeft);

    expireReadMailSB = new KPluralHandlingSpinBox(this);
    expireReadMailSB->setMaximum(999999);
    expireReadMailSB->setValue(30);
    expireReadMailSB->setSuffix(ki18ncp("Expire messages after %1", " day", " days"));
    daysBox->addWidget(expireReadMailSB, 0, 1);
    connect(expireReadMailSB, QOverload<int>::of(&KPluralHandlingSpinBox::valueChanged), this, &CollectionExpiryWidget::slotChanged);

    expireUnreadMailCB = new QCheckBox(this);
    expireUnreadMailCB->setText(i18n("Expire unread messages after"));
    connect(expireUnreadMailCB, &QCheckBox::toggled, this, &CollectionExpiryWidget::slotUpdateControls);
    daysBox->addWidget(expireUnreadMailCB, 1, 0, Qt::AlignLeft);

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

CollectionExpirySettings CollectionExpiryWidget::save()
{
    CollectionExpirySettings settings;
    settings.expiryGloballyOn = expireReadMailCB->isChecked() || expireUnreadMailCB->isChecked();
#if 0
    const bool enableGlobally = expireReadMailCB->isChecked() || expireUnreadMailCB->isChecked();
    const Akonadi::Collection expireToFolder = folderSelector->collection();
    if (enableGlobally && moveToRB->isChecked() && !expireToFolder.isValid()) {
        KMessageBox::error(this, i18n("Please select a folder to expire messages into.\nIf this is not done, expired messages will be permanently deleted."),
                           i18n("No Folder Selected"));
        deletePermanentlyRB->setChecked(true);
        expireNow = false;                                // settings are not valid
    }

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

    attribute->setAutoExpire(enableGlobally);
    // we always write out days now
    attribute->setReadExpireAge(expireReadMailSB->value());
    attribute->setUnreadExpireAge(expireUnreadMailSB->value());
    attribute->setReadExpireUnits(expireReadMailCB->isChecked() ? MailCommon::ExpireCollectionAttribute::ExpireDays
                                  : MailCommon::ExpireCollectionAttribute::ExpireNever);
    attribute->setUnreadExpireUnits(expireUnreadMailCB->isChecked() ? MailCommon::ExpireCollectionAttribute::ExpireDays
                                    : MailCommon::ExpireCollectionAttribute::ExpireNever);

    if (deletePermanentlyRB->isChecked()) {
        attribute->setExpireAction(ExpireCollectionAttribute::ExpireDelete);
    } else {
        attribute->setExpireAction(ExpireCollectionAttribute::ExpireMove);
    }
    if (saveSettings) {
        Akonadi::CollectionModifyJob *job = new Akonadi::CollectionModifyJob(collection, this);
        job->setProperty("expireNow", expireNow);
        connect(job, &Akonadi::CollectionModifyJob::result, this, &CollectionExpiryPage::slotCollectionModified);
    } else {
        if (expireNow) {
            MailCommon::Util::expireOldMessages(collection, true /*immediate*/);
        }
    }
    mChanged = false;
#endif
    //TODO
    return settings;
}
