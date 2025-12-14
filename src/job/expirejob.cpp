/**
 * SPDX-FileCopyrightText: 2004 David Faure <faure@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "expirejob.h"
#include "collectionpage/attributes/expirecollectionattribute.h"
#include "expiredeletejob.h"
#include "expiremovejob.h"
#include "kernel/mailkernel.h"

#include <PimCommon/BroadcastStatus>
using PimCommon::BroadcastStatus;
#include "mailcommon_debug.h"

#include <KLocalizedString>

#include <Akonadi/ItemDeleteJob>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/ItemModifyJob>
#include <Akonadi/ItemMoveJob>
#include <Akonadi/MessageFlags>
#include <Akonadi/MessageParts>
#include <Akonadi/MessageStatus>
#include <KMime/Message>

/*
 Testcases for folder expiry:
  Automatic expiry:
  - normal case (ensure folder has old mails and expiry settings, wait for auto-expiry)
  - having the folder selected when the expiry job would run (gets delayed)
  - selecting a folder while an expiry job is running for it (should interrupt)
  - exiting kmail while an expiry job is running (should abort & delete things cleanly)
  Manual expiry:
  - RMB / expire (for one folder)                   [KMMainWidget::slotExpireFolder()]
  - RMB on Local Folders / Expire All Folders       [KMFolderMgr::expireAll()]
  - Expire All Folders                              [KMMainWidget::slotExpireAll()]
*/

using namespace MailCommon;
ExpireJob::ExpireJob(const Akonadi::Collection &folder, bool immediate)
    : ScheduledJob(folder, immediate)
{
}

ExpireJob::~ExpireJob()
{
    qCDebug(MAILCOMMON_LOG);
}

void ExpireJob::kill()
{
    ScheduledJob::kill();
}

void ExpireJob::execute()
{
    const MailCommon::ExpireCollectionAttribute *expirationAttribute = mSrcFolder.attribute<MailCommon::ExpireCollectionAttribute>();
    if (expirationAttribute) {
        mMaxUnreadTime = 0;
        mMaxReadTime = 0;
        int unreadDays;
        int readDays;
        mExpireMessagesWithoutInvalidDate = expirationAttribute->expireMessagesWithValidDate();
        expirationAttribute->daysToExpire(unreadDays, readDays);

        if (unreadDays > 0) {
            qCDebug(MAILCOMMON_LOG) << "ExpireJob: deleting unread older than" << unreadDays << "days";
            mMaxUnreadTime = QDateTime::currentSecsSinceEpoch() - unreadDays * 3600 * 24;
        }
        if (readDays > 0) {
            qCDebug(MAILCOMMON_LOG) << "ExpireJob: deleting read older than" << readDays << "days";
            mMaxReadTime = QDateTime::currentSecsSinceEpoch() - readDays * 3600 * 24;
        }

        if ((mMaxUnreadTime == 0) && (mMaxReadTime == 0)) {
            qCDebug(MAILCOMMON_LOG) << "ExpireJob: nothing to do";
            deleteLater();
            return;
        }
    } else {
        deleteLater();
        return;
    }
    qCDebug(MAILCOMMON_LOG) << "ExpireJob: starting to expire in folder" << mSrcFolder.name();
    slotDoWork();
    // do nothing here, we might be deleted!
}

void ExpireJob::slotDoWork()
{
    auto job = new Akonadi::ItemFetchJob(mSrcFolder, this);
    job->fetchScope().fetchPayloadPart(Akonadi::MessagePart::Envelope);
    connect(job, &Akonadi::ItemFetchJob::result, this, &ExpireJob::itemFetchResult);
}

void ExpireJob::itemFetchResult(KJob *job)
{
    if (job->error()) {
        qCWarning(MAILCOMMON_LOG) << job->errorString();
        deleteLater();
        return;
    }

    const Akonadi::Item::List items = qobject_cast<Akonadi::ItemFetchJob *>(job)->items();
    for (const Akonadi::Item &item : items) {
        if (!item.hasPayload<QSharedPointer<KMime::Message>>()) {
            continue;
        }

        const auto mb = item.payload<QSharedPointer<KMime::Message>>();
        Akonadi::MessageStatus status;
        status.setStatusFromFlags(item.flags());
        if ((status.isImportant() || status.isToAct() || status.isWatched()) && SettingsIf->excludeImportantMailFromExpiry()) {
            continue;
        }

        auto mailDate = mb->date(false);
        if (!mailDate) {
            if (mExpireMessagesWithoutInvalidDate) {
                mRemovedMsgs.append(item);
            }
        } else {
            const time_t maxTime = status.isRead() ? mMaxReadTime : mMaxUnreadTime;
            if (mailDate->dateTime().toSecsSinceEpoch() < maxTime) {
                mRemovedMsgs.append(item);
            }
        }
    }

    done();
}

void ExpireJob::done()
{
    bool moving = false;
    if (!mRemovedMsgs.isEmpty()) {
        const int count = mRemovedMsgs.count();

        // The command shouldn't kill us because it opens the folder
        mCancellable = false;

        const MailCommon::ExpireCollectionAttribute *expirationAttribute = mSrcFolder.attribute<MailCommon::ExpireCollectionAttribute>();
        QString str;
        if (expirationAttribute) {
            const QString srcFolderName{mSrcFolder.name()};
            if (expirationAttribute->expireAction() == MailCommon::ExpireCollectionAttribute::ExpireDelete) {
                // Expire by deletion, i.e. move to null target folder
                qCDebug(MAILCOMMON_LOG) << "ExpireJob: finished expiring in folder" << srcFolderName << count << "messages to remove.";
                auto job = new ExpireDeleteJob(this);
                job->setRemovedMsgs(mRemovedMsgs);
                job->setSourceFolderName(srcFolderName);
                connect(job, &ExpireDeleteJob::expireDeleteDone, this, &ExpireJob::slotExpireDeleteDone);
                moving = true;
                str = i18np("Removing 1 old message from folder %2…", "Removing %1 old messages from folder %2...", count, srcFolderName);
                job->start();
            } else {
                // Expire by moving
                mMoveToFolder = Kernel::self()->collectionFromId(expirationAttribute->expireToFolderId());
                if (!mMoveToFolder.isValid()) {
                    str = i18n(
                        "Cannot expire messages from folder %1: destination "
                        "folder %2 not found",
                        srcFolderName,
                        expirationAttribute->expireToFolderId());
                    qCWarning(MAILCOMMON_LOG) << str;
                } else {
                    qCDebug(MAILCOMMON_LOG) << "ExpireJob: finished expiring in folder" << srcFolderName << mRemovedMsgs.count() << "messages to move to"
                                            << mMoveToFolder.name();

                    auto job = new ExpireMoveJob(this);
                    job->setRemovedMsgs(mRemovedMsgs);
                    job->setSrcFolderName(srcFolderName);
                    job->setMoveToFolder(mMoveToFolder);
                    connect(job, &ExpireMoveJob::expireMovedDone, this, &ExpireJob::slotExpireDeleteDone);
                    job->start();
                    moving = true;
                    str = i18np("Moving 1 old message from folder %2 to folder %3…",
                                "Moving %1 old messages from folder %2 to folder %3…",
                                count,
                                srcFolderName,
                                mMoveToFolder.name());
                }
            }
        }
        if (!str.isEmpty()) {
            BroadcastStatus::instance()->setStatusMsg(str);
        }
    }

    if (!moving) {
        deleteLater();
    }
}

void ExpireJob::slotExpireDeleteDone()
{
    deleteLater();
}

#include "moc_expirejob.cpp"
