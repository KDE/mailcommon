/*
  SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "mailcommon_export.h"

#include <MailCommon/ExpireCollectionAttribute>
#include <QWidget>
class QCheckBox;
class QSpinBox;
class QRadioButton;
class QPushButton;
namespace MailCommon
{
class FolderRequester;
struct MAILCOMMON_EXPORT CollectionExpirySettings {
    [[nodiscard]] bool isValid() const;

    bool expiryGloballyOn = false;
    bool expiryMessagesWithInvalidDate = false;
    int daysToExpireRead = -1;
    int daysToExpireUnread = -1;
    ExpireCollectionAttribute::ExpireUnits mUnreadExpireUnits = ExpireCollectionAttribute::ExpireNever;
    ExpireCollectionAttribute::ExpireUnits mReadExpireUnits = ExpireCollectionAttribute::ExpireNever;
    ExpireCollectionAttribute::ExpireAction mExpireAction = ExpireCollectionAttribute::ExpireDelete;
    Akonadi::Collection::Id mExpireToFolderId = -1;

    void convertFromExpireCollectionAttribute(const ExpireCollectionAttribute *attr);
};

/*!
 * \class MailCommon::CollectionExpiryWidget
 * \inmodule MailCommon
 * \inheaderfile MailCommon/CollectionExpiryWidget
 *
 * \brief The CollectionExpiryWidget class
 */
class MAILCOMMON_EXPORT CollectionExpiryWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     */
    explicit CollectionExpiryWidget(QWidget *parent = nullptr);
    /*!
     */
    ~CollectionExpiryWidget() override;

    /*!
     */
    void load(const MailCommon::CollectionExpirySettings &settings);
    /*!
     */
    void save(Akonadi::Collection &collection, bool saveSettings, bool expireNow);
    /*!
     */
    void save(const CollectionExpirySettings &collectionExpirySettings, Akonadi::Collection &collection, bool saveSettings, bool expireNow);
    /*!
     */
    [[nodiscard]] CollectionExpirySettings settings() const;
    /*!
     */
    void hideExpireNowButton();
    /*!
     */
    [[nodiscard]] static bool canHandle(const Akonadi::Collection &col);
Q_SIGNALS:
    /*!
     */
    void saveAndExpireRequested();
    /*!
     */
    void configChanged(bool changed = true);

private:
    [[nodiscard]] MAILCOMMON_NO_EXPORT bool validateExpireFolder(bool expireNow);
    [[nodiscard]] MAILCOMMON_NO_EXPORT MailCommon::ExpireCollectionAttribute *assignFolderAttribute(Akonadi::Collection &collection, bool &expireNow);
    MAILCOMMON_NO_EXPORT void slotChanged();
    QSpinBox *const mExpireReadMailSB;
    QSpinBox *const mExpireUnreadMailSB;
    QRadioButton *mMoveToRB = nullptr;
    FolderRequester *const mFolderSelector;
    QRadioButton *mDeletePermanentlyRB = nullptr;
    QPushButton *const mExpireNowPB;
    QCheckBox *const mExpireMailWithInvalidDateCB;
};
}
MAILCOMMON_EXPORT QDebug operator<<(QDebug d, const MailCommon::CollectionExpirySettings &t);
