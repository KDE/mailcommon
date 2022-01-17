/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "mailcommon_export.h"

#include <MailCommon/ExpireCollectionAttribute>
#include <QWidget>
class QCheckBox;
class KPluralHandlingSpinBox;
class QRadioButton;
class QPushButton;
namespace MailCommon
{
class FolderRequester;
struct MAILCOMMON_EXPORT CollectionExpirySettings {
    Q_REQUIRED_RESULT bool isValid() const;

    bool expiryGloballyOn = false;
    bool expiryMessagesWithInvalidDate = false;
    int daysToExpireRead = -1;
    int daysToExpireUnread = -1;
    ExpireCollectionAttribute::ExpireUnits mUnreadExpireUnits = ExpireCollectionAttribute::ExpireNever;
    ExpireCollectionAttribute::ExpireUnits mReadExpireUnits = ExpireCollectionAttribute::ExpireNever;
    ExpireCollectionAttribute::ExpireAction mExpireAction = ExpireCollectionAttribute::ExpireDelete;
    Akonadi::Collection::Id mExpireToFolderId = -1;
};

class MAILCOMMON_EXPORT CollectionExpiryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CollectionExpiryWidget(QWidget *parent = nullptr);
    ~CollectionExpiryWidget() override;

    void load(const MailCommon::CollectionExpirySettings &settings);
    void save(Akonadi::Collection &collection, bool saveSettings, bool expireNow);
    void save(const CollectionExpirySettings &collectionExpirySettings, Akonadi::Collection &collection, bool saveSettings, bool expireNow);
    Q_REQUIRED_RESULT CollectionExpirySettings settings() const;
    void hideExpireNowButton();
    Q_REQUIRED_RESULT static bool canHandle(const Akonadi::Collection &col);
Q_SIGNALS:
    void saveAndExpireRequested();
    void configChanged(bool changed = true);

private:
    Q_REQUIRED_RESULT bool validateExpireFolder(bool expireNow);
    Q_REQUIRED_RESULT MailCommon::ExpireCollectionAttribute *assignFolderAttribute(Akonadi::Collection &collection, bool &expireNow);
    void slotChanged();
    KPluralHandlingSpinBox *const mExpireReadMailSB;
    KPluralHandlingSpinBox *const mExpireUnreadMailSB;
    QRadioButton *mMoveToRB = nullptr;
    FolderRequester *const mFolderSelector;
    QRadioButton *mDeletePermanentlyRB = nullptr;
    QPushButton *const mExpireNowPB;
    QCheckBox *const mExpireMailWithInvalidDateCB;
};
}
MAILCOMMON_EXPORT QDebug operator<<(QDebug d, const MailCommon::CollectionExpirySettings &t);
