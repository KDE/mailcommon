/**
 * SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "expiremovejob.h"
#include "mailcommon_debug.h"
#include <Akonadi/ItemMoveJob>

ExpireMoveJob::ExpireMoveJob(QObject *parent)
    : QObject{parent}
{
}

ExpireMoveJob::~ExpireMoveJob() = default;

Akonadi::Item::List ExpireMoveJob::removedMsgs() const
{
    return mRemovedMsgs;
}

void ExpireMoveJob::setRemovedMsgs(const Akonadi::Item::List &newRemovedMsgs)
{
    mRemovedMsgs = newRemovedMsgs;
}

void ExpireMoveJob::start()
{
    if (mRemovedMsgs.isEmpty()) {
        qCDebug(MAILCOMMON_LOG) << "ExpireMoveJob: nothing to do. Bug ?";
        finished();
        return;
    }
    Akonadi::Item::List ids;
    ids.reserve(100);
    for (const Akonadi::Item &item : mRemovedMsgs) {
        ids.append(item);
        if (ids.count() >= 100) {
            auto job = new Akonadi::ItemMoveJob(ids, mMoveToFolder, this);
            connect(job, &Akonadi::ItemMoveJob::result, this, &ExpireMoveJob::slotMoveDone);

            mRunningJobs.append(job);
            ids.clear();
            ids.reserve(100);
        }
    }
    if (!ids.isEmpty()) {
        auto job = new Akonadi::ItemMoveJob(ids, mMoveToFolder, this);
        mRunningJobs.append(job);
        connect(job, &Akonadi::ItemMoveJob::result, this, &ExpireMoveJob::slotMoveDone);
    }
}

void ExpireMoveJob::finished()
{
    Q_EMIT expireMovedDone();
    deleteLater();
}

Akonadi::Collection ExpireMoveJob::moveToFolder() const
{
    return mMoveToFolder;
}

void ExpireMoveJob::setMoveToFolder(const Akonadi::Collection &newMoveToFolder)
{
    mMoveToFolder = newMoveToFolder;
}

void ExpireMoveJob::slotMoveDone(KJob *job)
{
    // TODO

    mRunningJobs.removeOne(job);
    if (mRunningJobs.isEmpty()) {
        finished();
    }
}

#include "moc_expiremovejob.cpp"
