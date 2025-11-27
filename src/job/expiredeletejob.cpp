/**
 * SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "expiredeletejob.h"
#include "mailcommon_debug.h"
#include <Akonadi/ItemDeleteJob>
#include <KLocalizedString>
#include <PimCommon/BroadcastStatus>
using PimCommon::BroadcastStatus;

ExpireDeleteJob::ExpireDeleteJob(QObject *parent)
    : QObject{parent}
{
}

ExpireDeleteJob::~ExpireDeleteJob() = default;

Akonadi::Item::List ExpireDeleteJob::removedMsgs() const
{
    return mRemovedMsgs;
}

void ExpireDeleteJob::setRemovedMsgs(const Akonadi::Item::List &newRemovedMsgs)
{
    mRemovedMsgs = newRemovedMsgs;
}

void ExpireDeleteJob::start()
{
    if (mRemovedMsgs.isEmpty()) {
        qCDebug(MAILCOMMON_LOG) << "ExpireDeleteJob: nothing to do";
        finished();
        return;
    }
    Akonadi::Item::List ids;
    ids.reserve(100);
    for (const Akonadi::Item &item : std::as_const(mRemovedMsgs)) {
        ids.append(item);
        if (ids.count() >= 100) {
            auto job = new Akonadi::ItemDeleteJob(ids, this);
            mRunningJobs.append(job);
            connect(job, &Akonadi::ItemDeleteJob::result, this, &ExpireDeleteJob::slotExpireDone);
            ids.clear();
            ids.reserve(100);
        }
    }
    if (!ids.isEmpty()) {
        auto job = new Akonadi::ItemDeleteJob(ids, this);
        mRunningJobs.append(job);
        connect(job, &Akonadi::ItemDeleteJob::result, this, &ExpireDeleteJob::slotExpireDone);
    }
}

void ExpireDeleteJob::slotExpireDone(KJob *job)
{
    if (job->error()) {
        qCCritical(MAILCOMMON_LOG) << job->error() << job->errorString();
    }
    QString msg;
    const int error = job->error();
    switch (error) {
    case KJob::NoError:
        msg = i18np("Removed 1 old message from folder %2.", "Removed %1 old messages from folder %2.", mRemovedMsgs.count(), mSourceFolderName);
        break;

    case Akonadi::Job::UserCanceled:
        msg = i18n("Removing old messages from folder %1 was canceled.", mSourceFolderName);
        break;

    default: // any other error
        msg = i18n("Removing old messages from folder %1 failed.", mSourceFolderName);
        break;
    }

    // TODO cancel all job when user cancel it.
    // Return when we have an error.

    mRunningJobs.removeOne(job);
    if (mRunningJobs.isEmpty()) {
        finished();
    }
    BroadcastStatus::instance()->setStatusMsg(msg);
}

QString ExpireDeleteJob::sourceFolderName() const
{
    return mSourceFolderName;
}

void ExpireDeleteJob::setSourceFolderName(const QString &newSourceFolderName)
{
    mSourceFolderName = newSourceFolderName;
}

void ExpireDeleteJob::finished()
{
    Q_EMIT expireDeleteDone();
    deleteLater();
}

#include "moc_expiredeletejob.cpp"
