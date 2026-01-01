/*
  SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "collectionexpirywidget.h"
#include "attributes/expirecollectionattribute.h"
#include "collectionexpiryjob.h"
#include "folder/foldersettings.h"
#include "folderrequester.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"
#include <KLocalization>

#include <Akonadi/CollectionModifyJob>

#include <KMessageBox>
#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>

using namespace MailCommon;

class DaysSpinBox : public QSpinBox
{
public:
    explicit DaysSpinBox(QWidget *parent)
        : QSpinBox(parent)
    {
        setMaximum(999999);
        KLocalization::setupSpinBoxFormatString(this, ki18ncp("Expire messages after %1", "%v day", "%v days"));
        setSpecialValueText(i18n("Never"));
    }

    [[nodiscard]] QString textFromValue(int value) const override
    {
        if (value == 0) {
            return i18n("Never");
        }
        return QSpinBox::textFromValue(value);
    }

    [[nodiscard]] int valueFromText(const QString &text) const override
    {
        return QSpinBox::valueFromText(text);
    }

    QValidator::State validate(QString &text, int &pos) const override
    {
        if (text == i18n("Never")) {
            return QValidator::Acceptable;
        }
        return QSpinBox::validate(text, pos);
    }
};

CollectionExpiryWidget::CollectionExpiryWidget(QWidget *parent)
    : QWidget(parent)
    , mExpireReadMailSB(new DaysSpinBox(this))
    , mExpireUnreadMailSB(new DaysSpinBox(this))
    , mFolderSelector(new FolderRequester(this))
    , mExpireNowPB(new QPushButton(i18nc("@action:button", "Save Settings and Expire Now"), this))
    , mExpireMailWithInvalidDateCB(new QCheckBox(i18nc("@option:check", "Expire messages with invalid date"), this))
{
    auto formLayout = new QFormLayout(this);
    formLayout->setContentsMargins({});

    connect(mExpireReadMailSB, &QSpinBox::valueChanged, this, &CollectionExpiryWidget::slotChanged);
    formLayout->addRow(i18n("Expire read messages after:"), mExpireReadMailSB);

    connect(mExpireUnreadMailSB, &QSpinBox::valueChanged, this, &CollectionExpiryWidget::slotChanged);
    formLayout->addRow(i18n("Expire unread messages after:"), mExpireUnreadMailSB);

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

    mFolderSelector->setMustBeReadWrite(true);
    mFolderSelector->setShowOutbox(false);
    moveToHBox->addWidget(mFolderSelector);
    formLayout->addRow(QString(), moveToHBox);
    connect(mFolderSelector, &FolderRequester::folderChanged, this, &CollectionExpiryWidget::slotChanged);

    mDeletePermanentlyRB = new QRadioButton(actionsGroup);
    mDeletePermanentlyRB->setText(i18n("Delete expired messages permanently"));
    connect(mDeletePermanentlyRB, &QRadioButton::toggled, this, &CollectionExpiryWidget::slotChanged);

    formLayout->addRow(QString(), mDeletePermanentlyRB);

    connect(mExpireNowPB, &QPushButton::clicked, this, &CollectionExpiryWidget::saveAndExpireRequested);
    formLayout->addRow(QString(), mExpireNowPB);

    mDeletePermanentlyRB->setChecked(true);
    slotChanged();
}

CollectionExpiryWidget::~CollectionExpiryWidget() = default;

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
                           i18nc("@title:window", "No Folder Selected"));
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
                               i18nc("@title:window", "Wrong Folder Selected"));
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
    attribute->setExpireMessagesWithValidDate(!collectionExpirySettings.expiryMessagesWithInvalidDate);

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

void CollectionExpirySettings::convertFromExpireCollectionAttribute(const ExpireCollectionAttribute *attr)
{
    if (attr) {
        attr->daysToExpire(daysToExpireUnread, daysToExpireRead);
        expiryGloballyOn = attr->isAutoExpire();
        mUnreadExpireUnits = attr->unreadExpireUnits();
        mReadExpireUnits = attr->readExpireUnits();
        mExpireAction = attr->expireAction();
        mExpireToFolderId = attr->expireToFolderId();
        expiryMessagesWithInvalidDate = !attr->expireMessagesWithValidDate();
    }
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

#include "moc_collectionexpirywidget.cpp"
