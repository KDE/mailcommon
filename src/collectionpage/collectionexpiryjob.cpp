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

#include "collectionexpiryjob.h"
#include "util/mailutil.h"
#include "mailcommon_debug.h"

#include <AkonadiCore/Collection>
#include <AkonadiCore/CollectionModifyJob>

using namespace MailCommon;

CollectionExpiryJob::CollectionExpiryJob(QObject *parent)
    : QObject(parent)
{

}

CollectionExpiryJob::~CollectionExpiryJob()
{

}

void CollectionExpiryJob::start()
{
    if (mCollection.isValid()) {
        Akonadi::CollectionModifyJob *job = new Akonadi::CollectionModifyJob(mCollection, this);
        connect(job, &Akonadi::CollectionModifyJob::result, this, &CollectionExpiryJob::slotCollectionModified);
    } else {
        qCWarning(MAILCOMMON_LOG) << "Collection is not valid";
        deleteLater();
    }
}

void CollectionExpiryJob::setCollection(const Akonadi::Collection &collection)
{
    mCollection = collection;
}

void CollectionExpiryJob::setExpireNow(bool expireNow)
{
    mExpireNow = expireNow;
}

void CollectionExpiryJob::slotCollectionModified(KJob *job)
{
    if (job->error()) {
        qCWarning(MAILCOMMON_LOG) << " Error when we modified collection";
        deleteLater();
        return;
    }

    // trigger immediate expiry if there is something to do
    if (mExpireNow) {
        Akonadi::CollectionModifyJob *modifiedJob = qobject_cast<Akonadi::CollectionModifyJob *>(job);
        if (modifiedJob) {
            MailCommon::Util::expireOldMessages(modifiedJob->collection(), true /*immediate*/);
        } else {
            qCWarning(MAILCOMMON_LOG) << " invalid pointer";
        }
    }
    deleteLater();
}
