/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "collectionexpiryjob.h"
#include "mailcommon_debug.h"
#include "util/mailutil.h"

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
        auto job = new Akonadi::CollectionModifyJob(mCollection, this);
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
        auto modifiedJob = qobject_cast<Akonadi::CollectionModifyJob *>(job);
        if (modifiedJob) {
            MailCommon::Util::expireOldMessages(modifiedJob->collection(), true /*immediate*/);
        } else {
            qCWarning(MAILCOMMON_LOG) << " invalid pointer";
        }
    }
    deleteLater();
}
