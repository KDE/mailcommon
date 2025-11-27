/**
 * SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "expiremovejob.h"
#include "akonadi/messageflags.h"
#include "mailcommon_debug.h"
#include <Akonadi/ItemModifyJob>
#include <Akonadi/ItemMoveJob>
#include <KLocalizedString>
#include <PimCommon/BroadcastStatus>

using PimCommon::BroadcastStatus;

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
    for (const Akonadi::Item &item : std::as_const(mRemovedMsgs)) {
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

QString ExpireMoveJob::srcFolderName() const
{
    return mSrcFolderName;
}

void ExpireMoveJob::setSrcFolderName(const QString &newSrcFolderName)
{
    mSrcFolderName = newSrcFolderName;
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
    if (job->error()) {
        qCCritical(MAILCOMMON_LOG) << job->error() << job->errorString();
        deleteLater();
        return;
    }
    auto itemjob = qobject_cast<Akonadi::ItemMoveJob *>(job);
    if (itemjob) {
        const Akonadi::Item::List lst = itemjob->items();
        if (!lst.isEmpty()) {
            Akonadi::Item::List newLst;
            for (Akonadi::Item item : lst) {
                if (!item.hasFlag(Akonadi::MessageFlags::Seen)) {
                    item.setFlag(Akonadi::MessageFlags::Seen);
                    newLst << item;
                }
            }
            if (!newLst.isEmpty()) {
                auto modifyJob = new Akonadi::ItemModifyJob(newLst, this);
                modifyJob->disableRevisionCheck();
                connect(modifyJob, &Akonadi::ItemModifyJob::result, this, &ExpireMoveJob::slotExpireDone);
            } else {
                slotExpireDone(job);
            }
        }
    } else {
        slotExpireDone(job);
    }
}

void ExpireMoveJob::slotExpireDone(KJob *job)
{
    if (job->error()) {
        qCCritical(MAILCOMMON_LOG) << job->error() << job->errorString();
    }
    QString msg;
    const int error = job->error();
    switch (error) {
    case KJob::NoError:
        msg = i18np("Moved 1 old message from folder %2 to folder %3.",
                    "Moved %1 old messages from folder %2 to folder %3.",
                    mRemovedMsgs.count(),
                    mSrcFolderName,
                    mMoveToFolder.name());
        break;

    case Akonadi::Job::UserCanceled:
        msg = i18n(
            "Moving old messages from folder %1 to folder %2 was "
            "canceled.",
            mSrcFolderName,
            mMoveToFolder.name());
        break;

    default: // any other error
        msg = i18n("Moving old messages from folder %1 to folder %2 failed.", mSrcFolderName, mMoveToFolder.name());
        break;
    }

    mRunningJobs.removeOne(job);
    if (mRunningJobs.isEmpty()) {
        finished();
    }
    BroadcastStatus::instance()->setStatusMsg(msg);
}

#include "moc_expiremovejob.cpp"
