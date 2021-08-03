/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COLLECTIONEXPIRYWIDGET_H
#define COLLECTIONEXPIRYWIDGET_H
#include "mailcommon_export.h"

#include <MailCommon/ExpireCollectionAttribute>
#include <QWidget>
class QCheckBox;
class KPluralHandlingSpinBox;
class QRadioButton;
class QPushButton;
class KJob;
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
    static Q_REQUIRED_RESULT bool canHandle(const Akonadi::Collection &col);
Q_SIGNALS:
    void saveAndExpireRequested();
    void configChanged(bool changed = true);

private:
    Q_REQUIRED_RESULT bool validateExpireFolder(bool expireNow);
    Q_REQUIRED_RESULT MailCommon::ExpireCollectionAttribute *assignFolderAttribute(Akonadi::Collection &collection, bool &expireNow);
    void slotChanged();
    void slotUpdateControls();
    QCheckBox *mExpireReadMailCB = nullptr;
    KPluralHandlingSpinBox *mExpireReadMailSB = nullptr;
    QCheckBox *mExpireUnreadMailCB = nullptr;
    KPluralHandlingSpinBox *mExpireUnreadMailSB = nullptr;
    QRadioButton *mMoveToRB = nullptr;
    FolderRequester *mFolderSelector = nullptr;
    QRadioButton *mDeletePermanentlyRB = nullptr;
    QPushButton *mExpireNowPB = nullptr;
    QCheckBox *mExpireMailWithInvalidDateCB = nullptr;
};
}
MAILCOMMON_EXPORT QDebug operator<<(QDebug d, const MailCommon::CollectionExpirySettings &t);

#endif // COLLECTIONEXPIRYWIDGET_H
