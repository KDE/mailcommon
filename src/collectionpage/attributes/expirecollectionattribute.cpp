/*

  SPDX-FileCopyrightText: 2011-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "expirecollectionattribute.h"
#include "folder/foldersettings.h"
#include "kernel/mailkernel.h"

#include <QDataStream>

using namespace MailCommon;

ExpireCollectionAttribute::ExpireCollectionAttribute()
{
}

QByteArray ExpireCollectionAttribute::type() const
{
    static const QByteArray sType("expirationcollectionattribute");
    return sType;
}

ExpireCollectionAttribute *ExpireCollectionAttribute::clone() const
{
    auto expireAttr = new ExpireCollectionAttribute();
    expireAttr->setAutoExpire(mExpireMessages);
    expireAttr->setUnreadExpireAge(mUnreadExpireAge);
    expireAttr->setUnreadExpireUnits(mUnreadExpireUnits);
    expireAttr->setReadExpireAge(mReadExpireAge);
    expireAttr->setReadExpireUnits(mReadExpireUnits);
    expireAttr->setExpireAction(mExpireAction);
    expireAttr->setExpireToFolderId(mExpireToFolderId);
    expireAttr->setExpireMessagesWithValidDate(mExpireMessagesWithValidDate);
    return expireAttr;
}

void ExpireCollectionAttribute::setAutoExpire(bool enabled)
{
    mExpireMessages = enabled;
}

bool ExpireCollectionAttribute::isAutoExpire() const
{
    return mExpireMessages;
}

void ExpireCollectionAttribute::setUnreadExpireAge(int age)
{
    if (age >= 0 && age != mUnreadExpireAge) {
        mUnreadExpireAge = age;
    }
}

int ExpireCollectionAttribute::unreadExpireAge() const
{
    return mUnreadExpireAge;
}

void ExpireCollectionAttribute::setUnreadExpireUnits(ExpireUnits units)
{
    if (units >= ExpireNever && units < ExpireMaxUnits) {
        mUnreadExpireUnits = units;
    }
}

void ExpireCollectionAttribute::setReadExpireAge(int age)
{
    if (age >= 0 && age != mReadExpireAge) {
        mReadExpireAge = age;
    }
}

int ExpireCollectionAttribute::readExpireAge() const
{
    return mReadExpireAge;
}

void ExpireCollectionAttribute::setReadExpireUnits(ExpireUnits units)
{
    if (units >= ExpireNever && units <= ExpireMaxUnits) {
        mReadExpireUnits = units;
    }
}

void ExpireCollectionAttribute::setExpireAction(ExpireAction a)
{
    mExpireAction = a;
}

ExpireCollectionAttribute::ExpireAction ExpireCollectionAttribute::expireAction() const
{
    return mExpireAction;
}

void ExpireCollectionAttribute::setExpireToFolderId(Akonadi::Collection::Id id)
{
    mExpireToFolderId = id;
}

Akonadi::Collection::Id ExpireCollectionAttribute::expireToFolderId() const
{
    return mExpireToFolderId;
}

ExpireCollectionAttribute::ExpireUnits ExpireCollectionAttribute::unreadExpireUnits() const
{
    return mUnreadExpireUnits;
}

ExpireCollectionAttribute::ExpireUnits ExpireCollectionAttribute::readExpireUnits() const
{
    return mReadExpireUnits;
}

bool ExpireCollectionAttribute::operator==(const ExpireCollectionAttribute &other) const
{
    return (mExpireMessages == other.isAutoExpire()) && (mUnreadExpireAge == other.unreadExpireAge()) && (mReadExpireAge == other.readExpireAge())
        && (mUnreadExpireUnits == other.unreadExpireUnits()) && (mReadExpireUnits == other.readExpireUnits()) && (mExpireAction == other.expireAction())
        && (mExpireToFolderId == other.expireToFolderId()) && (mExpireMessagesWithValidDate == other.expireMessagesWithValidDate());
}

int ExpireCollectionAttribute::daysToExpire(int number, ExpireCollectionAttribute::ExpireUnits units)
{
    switch (units) {
    case ExpireCollectionAttribute::ExpireDays: // Days
        return number;
    case ExpireCollectionAttribute::ExpireWeeks: // Weeks
        return number * 7;
    case ExpireCollectionAttribute::ExpireMonths: // Months - this could be better
        // rather than assuming 31day months.
        return number * 31;
    default: // this avoids a compiler warning (not handled enumeration values)
             ;
    }
    return -1;
}

bool ExpireCollectionAttribute::expireMessagesWithValidDate() const
{
    return mExpireMessagesWithValidDate;
}

void ExpireCollectionAttribute::setExpireMessagesWithValidDate(bool expireMessagesWithValidDate)
{
    mExpireMessagesWithValidDate = expireMessagesWithValidDate;
}

void ExpireCollectionAttribute::daysToExpire(int &unreadDays, int &readDays) const
{
    unreadDays = ExpireCollectionAttribute::daysToExpire(unreadExpireAge(), unreadExpireUnits());
    readDays = ExpireCollectionAttribute::daysToExpire(readExpireAge(), readExpireUnits());
}

QByteArray ExpireCollectionAttribute::serialized() const
{
    QByteArray result;
    QDataStream s(&result, QIODevice::WriteOnly);

    s << mExpireToFolderId;
    s << static_cast<int>(mExpireAction);
    s << static_cast<int>(mReadExpireUnits);
    s << mReadExpireAge;
    s << static_cast<int>(mUnreadExpireUnits);
    s << mUnreadExpireAge;
    s << mExpireMessages;
    s << mExpireMessagesWithValidDate;

    return result;
}

void ExpireCollectionAttribute::deserialize(const QByteArray &data)
{
    QDataStream s(data);
    s >> mExpireToFolderId;
    int action;
    s >> action;
    mExpireAction = static_cast<ExpireCollectionAttribute::ExpireAction>(action);
    int valUnitRead;
    s >> valUnitRead;
    mReadExpireUnits = static_cast<ExpireCollectionAttribute::ExpireUnits>(valUnitRead);
    s >> mReadExpireAge;
    int valUnitUnread;
    s >> valUnitUnread;
    mUnreadExpireUnits = static_cast<ExpireCollectionAttribute::ExpireUnits>(valUnitUnread);
    s >> mUnreadExpireAge;
    s >> mExpireMessages;
    s >> mExpireMessagesWithValidDate;
}

QDebug operator<<(QDebug d, const ExpireCollectionAttribute &t)
{
    d << " mExpireMessages " << t.isAutoExpire();
    d << " mUnreadExpireAge " << t.unreadExpireAge();
    d << " mReadExpireAge " << t.readExpireAge();
    d << " mUnreadExpireUnits " << t.unreadExpireUnits();
    d << " mReadExpireUnits " << t.readExpireUnits();
    d << " mExpireAction " << t.expireAction();
    d << " mExpireToFolderId " << t.expireToFolderId();
    d << " mExpireMessagesWithValidDate " << t.expireMessagesWithValidDate();
    return d;
}
