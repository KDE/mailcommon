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
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>

using namespace MailCommon;

class DaysSpinBox : public KPluralHandlingSpinBox
{
public:
    DaysSpinBox(QWidget *parent)
        : KPluralHandlingSpinBox(parent)
    {
        setMaximum(999999);
        setSuffix(ki18ncp("Expire messages after %1", " day", " days"));
        setSpecialValueText(i18n("Never"));
    }

    QString textFromValue(int value) const override
    {
        if (value == 0) {
            return i18n("Never");
        }
        return KPluralHandlingSpinBox::textFromValue(value);
    }

    int valueFromText(const QString &text) const override
    {
        return KPluralHandlingSpinBox::valueFromText(text);
    }

    QValidator::State validate(QString &text, int &pos) const override
    {
        if (text == i18n("Never")) {
            return QValidator::Acceptable;
        }
        return KPluralHandlingSpinBox::validate(text, pos);
    }
};

CollectionExpiryWidget::CollectionExpiryWidget(QWidget *parent)
    : QWidget(parent)
{
    auto formLayout = new QFormLayout(this);
    formLayout->setContentsMargins({});

    mExpireReadMailSB = new DaysSpinBox(this);
    connect(mExpireReadMailSB, qOverload<int>(&KPluralHandlingSpinBox::valueChanged), this, &CollectionExpiryWidget::slotChanged);
    formLayout->addRow(i18n("Expire read messages after:"), mExpireReadMailSB);

    mExpireUnreadMailSB = new DaysSpinBox(this);
    connect(mExpireUnreadMailSB, qOverload<int>(&KPluralHandlingSpinBox::valueChanged), this, &CollectionExpiryWidget::slotChanged);
    formLayout->addRow(i18n("Expire unread messages after:"), mExpireUnreadMailSB);

    mExpireMailWithInvalidDateCB = new QCheckBox(i18n("Expire messages with invalid date"), this);
    connect(mExpireMailWithInvalidDateCB, &QCheckBox::toggled, this, &CollectionExpiryWidget::slotChanged);
    formLayout->addRow(QString(), mExpireMailWithInvalidDateCB);

    auto actionsGroup = new QGroupBox(this);
    actionsGroup->setFlat(true); // for mutual exclusion of the radio buttons
    formLayout->addRow(actionsGroup);

    auto moveToHBox = new QHBoxLayout();
    moveToHBox->setContentsMargins({});
    moveToHBox->setSpacing(6);

    mMoveToRB = new QRadioButton(actionsGroup);
    mMoveToRB->setText(i18n("Move expired messages to:"));
    connect(mMoveToRB, &QRadioButton::toggled, this, &CollectionExpiryWidget::slotChanged);
    moveToHBox->addWidget(mMoveToRB);

    mFolderSelector = new FolderRequester(this);
    mFolderSelector->setMustBeReadWrite(true);
    mFolderSelector->setShowOutbox(false);
    moveToHBox->addWidget(mFolderSelector);
    formLayout->addRow(QString(), moveToHBox);
    connect(mFolderSelector, &FolderRequester::folderChanged, this, &CollectionExpiryWidget::slotChanged);

    mDeletePermanentlyRB = new QRadioButton(actionsGroup);
    mDeletePermanentlyRB->setText(i18n("Delete expired messages permanently"));
    connect(mDeletePermanentlyRB, &QRadioButton::toggled, this, &CollectionExpiryWidget::slotChanged);

    formLayout->addRow(QString(), mDeletePermanentlyRB);

    mExpireNowPB = new QPushButton(i18n("Save Settings and Expire Now"), this);
    connect(mExpireNowPB, &QPushButton::clicked, this, &CollectionExpiryWidget::saveAndExpireRequested);
    formLayout->addRow(QString(), mExpireNowPB);

    mDeletePermanentlyRB->setChecked(true);
    slotChanged();
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
    const bool showExpiryActions = mExpireReadMailSB->value() != 0 || mExpireUnreadMailSB->value() != 0;
    mMoveToRB->setEnabled(showExpiryActions);
    mFolderSelector->setEnabled(showExpiryActions && mMoveToRB->isChecked());
    mDeletePermanentlyRB->setEnabled(showExpiryActions);
    mExpireNowPB->setEnabled(showExpiryActions);

    Q_EMIT configChanged();
}

void CollectionExpiryWidget::load(const MailCommon::CollectionExpirySettings &settings)
{
    if (settings.isValid()) {
        bool expiryGloballyOn = settings.expiryGloballyOn;
        if (expiryGloballyOn && settings.mReadExpireUnits != ExpireCollectionAttribute::ExpireNever && settings.daysToExpireRead >= 0) {
            mExpireReadMailSB->setValue(settings.daysToExpireRead);
        } else {
            mExpireReadMailSB->setValue(0);
        }
        if (expiryGloballyOn && settings.mUnreadExpireUnits != ExpireCollectionAttribute::ExpireNever && settings.daysToExpireUnread >= 0) {
            mExpireUnreadMailSB->setValue(settings.daysToExpireUnread);
        } else {
            mExpireUnreadMailSB->setValue(0);
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
    slotChanged();
}

bool CollectionExpiryWidget::validateExpireFolder(bool expireNow)
{
    const bool enableGlobally = mExpireReadMailSB->value() != 0 || mExpireUnreadMailSB->value() != 0;
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
    settings.expiryGloballyOn = mExpireReadMailSB->value() != 0 || mExpireUnreadMailSB->value() != 0;
    settings.expiryMessagesWithInvalidDate = mExpireMailWithInvalidDateCB->isChecked();
    // we always write out days now
    settings.daysToExpireRead = mExpireReadMailSB->value();
    settings.daysToExpireUnread = mExpireUnreadMailSB->value();
    settings.mReadExpireUnits =
        mExpireReadMailSB->value() != 0 ? MailCommon::ExpireCollectionAttribute::ExpireDays : MailCommon::ExpireCollectionAttribute::ExpireNever;
    settings.mUnreadExpireUnits =
        mExpireUnreadMailSB->value() != 0 ? MailCommon::ExpireCollectionAttribute::ExpireDays : MailCommon::ExpireCollectionAttribute::ExpireNever;

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
