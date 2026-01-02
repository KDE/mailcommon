/*

  SPDX-FileCopyrightText: 2011-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"

#include <Akonadi/Attribute>
#include <Akonadi/Collection>
#include <QDebug>

namespace MailCommon
{
class MAILCOMMON_EXPORT ExpireCollectionAttribute : public Akonadi::Attribute
{
public:
    ExpireCollectionAttribute();

    /*
     * Define the possible units to use for measuring message expiry.
     * expireNever is used to switch off message expiry, and expireMaxUnits
     * must always be the last in the list (for bounds checking).
     */
    enum ExpireUnits {
        ExpireNever = 0,
        ExpireDays = 1,
        ExpireWeeks = 2,
        ExpireMonths = 3,
        ExpireMaxUnits = 4,
    };

    enum ExpireAction {
        ExpireDelete = 0,
        ExpireMove = 1,
    };

    [[nodiscard]] QByteArray type() const override;
    ExpireCollectionAttribute *clone() const override;
    [[nodiscard]] QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

    void daysToExpire(int &unreadDays, int &readDays) const;

    /*!
     * Sets whether this folder automatically expires messages.
     */
    void setAutoExpire(bool enabled);

    /*!
     * Returns true if this folder automatically expires old messages.
     */
    [[nodiscard]] bool isAutoExpire() const;

    /*!
     * Sets the maximum age for unread messages in this folder.
     * Age should not be negative. Units are set using
     * setUnreadExpireUnits().
     */
    void setUnreadExpireAge(int age);

    /*!
     * Sets the units to use for expiry of unread messages.
     * Values are 1 = days, 2 = weeks, 3 = months.
     */
    void setUnreadExpireUnits(ExpireUnits units);

    /*!
     * Sets the maximum age for read messages in this folder.
     * Age should not be negative. Units are set using
     * setReadExpireUnits().
     */
    void setReadExpireAge(int age);

    /*!
     * Sets the units to use for expiry of read messages.
     * Values are 1 = days, 2 = weeks, 3 = months.
     */
    void setReadExpireUnits(ExpireUnits units);

    /*!
     * Returns the age at which unread messages are expired.
     * Units are determined by unreadExpireUnits().
     */
    [[nodiscard]] int unreadExpireAge() const;

    /*!
     * Returns the age at which read messages are expired.
     * Units are determined by readExpireUnits().
     */
    [[nodiscard]] int readExpireAge() const;

    /*!
     * What should expiry do? Delete or move to another folder?
     */
    [[nodiscard]] ExpireAction expireAction() const;
    void setExpireAction(ExpireAction a);

    /*!
     * If expiry should move to folder, return the ID of that folder
     */
    [[nodiscard]] Akonadi::Collection::Id expireToFolderId() const;
    void setExpireToFolderId(Akonadi::Collection::Id id);

    /*!
     * Units getUnreadExpireAge() is returned in.
     * 1 = days, 2 = weeks, 3 = months.
     */
    [[nodiscard]] ExpireUnits unreadExpireUnits() const;

    /*!
     * Units getReadExpireAge() is returned in.
     * 1 = days, 2 = weeks, 3 = months.
     */
    [[nodiscard]] ExpireUnits readExpireUnits() const;

    [[nodiscard]] bool operator==(const ExpireCollectionAttribute &other) const;
    [[nodiscard]] bool expireMessagesWithValidDate() const;
    void setExpireMessagesWithValidDate(bool expireMessagesWithValidDate);

private:
    static MAILCOMMON_NO_EXPORT int daysToExpire(int number, ExpireCollectionAttribute::ExpireUnits units);
    bool mExpireMessages = false; // true if old messages are expired
    int mUnreadExpireAge = 28; // Given in unreadExpireUnits
    int mReadExpireAge = 14; // Given in readExpireUnits
    ExpireCollectionAttribute::ExpireUnits mUnreadExpireUnits = ExpireNever;
    ExpireCollectionAttribute::ExpireUnits mReadExpireUnits = ExpireNever;
    ExpireCollectionAttribute::ExpireAction mExpireAction = ExpireDelete;
    Akonadi::Collection::Id mExpireToFolderId = -1;
    bool mExpireMessagesWithValidDate = false;
};
}
MAILCOMMON_EXPORT QDebug operator<<(QDebug d, const MailCommon::ExpireCollectionAttribute &t);
