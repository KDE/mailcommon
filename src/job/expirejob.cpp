/**
 * Copyright (c) 2004 David Faure <faure@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  In addition, as a special exception, the copyright holders give
 *  permission to link the code of this program with any edition of
 *  the Qt library by Trolltech AS, Norway (or with modified versions
 *  of Qt that use the same license as Qt), and distribute linked
 *  combinations including the two.  You must obey the GNU General
 *  Public License in all respects for all of the code used other than
 *  Qt.  If you modify this file, you may extend this exception to
 *  your version of the file, but you are not obligated to do so.  If
 *  you do not wish to do so, delete this exception statement from
 *  your version.
 */
#include "expirejob.h"
#include "collectionpage/attributes/expirecollectionattribute.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"

#include <Libkdepim/BroadcastStatus>
using KPIM::BroadcastStatus;
#include "mailcommon_debug.h"

#include <KLocalizedString>

#include <ItemDeleteJob>
#include <ItemModifyJob>
#include <ItemFetchJob>
#include <ItemFetchScope>
#include <ItemMoveJob>
#include <Akonadi/KMime/MessageParts>
#include <Akonadi/KMime/MessageStatus>
#include <Akonadi/KMime/MessageFlags>
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

namespace MailCommon {
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
    mMaxUnreadTime = 0;
    mMaxReadTime = 0;
    int unreadDays, readDays;

    const MailCommon::ExpireCollectionAttribute *expirationAttribute = mSrcFolder.attribute<MailCommon::ExpireCollectionAttribute>();
    if (expirationAttribute) {
        expirationAttribute->daysToExpire(unreadDays, readDays);

        if (unreadDays > 0) {
            qCDebug(MAILCOMMON_LOG) << "ExpireJob: deleting unread older than" << unreadDays << "days";
            mMaxUnreadTime = QDateTime::currentDateTime().toSecsSinceEpoch() - unreadDays * 3600 * 24;
        }
        if (readDays > 0) {
            qCDebug(MAILCOMMON_LOG) << "ExpireJob: deleting read older than" << readDays << "days";
            mMaxReadTime = QDateTime::currentDateTime().toSecsSinceEpoch() - readDays * 3600 * 24;
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
    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(mSrcFolder, this);
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
        if (!item.hasPayload<KMime::Message::Ptr>()) {
            continue;
        }

        const KMime::Message::Ptr mb = item.payload<KMime::Message::Ptr>();
        Akonadi::MessageStatus status;
        status.setStatusFromFlags(item.flags());
        if ((status.isImportant() || status.isToAct() || status.isWatched())
            && SettingsIf->excludeImportantMailFromExpiry()) {
            continue;
        }

        time_t maxTime = status.isRead() ? mMaxReadTime : mMaxUnreadTime;

        if (!mb->date(false)) {
            continue;
        }

        if (mb->date()->dateTime().toSecsSinceEpoch() < maxTime) {
            mRemovedMsgs.append(item);
        }
    }

    done();
}

void ExpireJob::done()
{
    QString str;
    bool moving = false;

    if (!mRemovedMsgs.isEmpty()) {
        int count = mRemovedMsgs.count();

        // The command shouldn't kill us because it opens the folder
        mCancellable = false;

        const MailCommon::ExpireCollectionAttribute *expirationAttribute = mSrcFolder.attribute<MailCommon::ExpireCollectionAttribute>();
        if (expirationAttribute) {
            if (expirationAttribute->expireAction() == MailCommon::ExpireCollectionAttribute::ExpireDelete) {
                // Expire by deletion, i.e. move to null target folder
                qCDebug(MAILCOMMON_LOG) << "ExpireJob: finished expiring in folder"
                                        << mSrcFolder.name()
                                        << count << "messages to remove.";
                Akonadi::ItemDeleteJob *job = new Akonadi::ItemDeleteJob(mRemovedMsgs, this);
                connect(job, &Akonadi::ItemDeleteJob::result, this, &ExpireJob::slotExpireDone);
                moving = true;
                str = i18np("Removing 1 old message from folder %2...",
                            "Removing %1 old messages from folder %2...",
                            count, mSrcFolder.name());
            } else {
                // Expire by moving
                mMoveToFolder = Kernel::self()->collectionFromId(expirationAttribute->expireToFolderId());
                if (!mMoveToFolder.isValid()) {
                    str = i18n("Cannot expire messages from folder %1: destination "
                               "folder %2 not found",
                               mSrcFolder.name(), expirationAttribute->expireToFolderId());
                    qCWarning(MAILCOMMON_LOG) << str;
                } else {
                    qCDebug(MAILCOMMON_LOG) << "ExpireJob: finished expiring in folder"
                                            << mSrcFolder.name()
                                            << mRemovedMsgs.count() << "messages to move to"
                                            << mMoveToFolder.name();
                    Akonadi::ItemMoveJob *job = new Akonadi::ItemMoveJob(mRemovedMsgs, mMoveToFolder, this);
                    connect(job, &Akonadi::ItemMoveJob::result, this, &ExpireJob::slotMoveDone);
                    moving = true;
                    str = i18np("Moving 1 old message from folder %2 to folder %3...",
                                "Moving %1 old messages from folder %2 to folder %3...",
                                count, mSrcFolder.name(), mMoveToFolder.name());
                }
            }
        }
    }
    if (!str.isEmpty()) {
        BroadcastStatus::instance()->setStatusMsg(str);
    }

    if (!moving) {
        deleteLater();
    }
}

void ExpireJob::slotMoveDone(KJob *job)
{
    if (job->error()) {
        qCCritical(MAILCOMMON_LOG) << job->error() << job->errorString();
    }
    Akonadi::ItemMoveJob *itemjob = qobject_cast<Akonadi::ItemMoveJob *>(job);
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
                Akonadi::ItemModifyJob *modifyJob = new Akonadi::ItemModifyJob(newLst, this);
                modifyJob->disableRevisionCheck();
                connect(modifyJob, &Akonadi::ItemModifyJob::result, this, &ExpireJob::slotExpireDone);
            } else {
                slotExpireDone(job);
            }
        }
    } else {
        slotExpireDone(job);
    }
}

void ExpireJob::slotExpireDone(KJob *job)
{
    if (job->error()) {
        qCCritical(MAILCOMMON_LOG) << job->error() << job->errorString();
    }

    QString msg;
    const int error = job->error();

    const MailCommon::ExpireCollectionAttribute *expirationAttribute = mSrcFolder.attribute<MailCommon::ExpireCollectionAttribute>();
    if (expirationAttribute) {
        switch (error) {
        case KJob::NoError:
            if (expirationAttribute->expireAction() == MailCommon::ExpireCollectionAttribute::ExpireDelete) {
                msg = i18np("Removed 1 old message from folder %2.",
                            "Removed %1 old messages from folder %2.",
                            mRemovedMsgs.count(),
                            mSrcFolder.name());
            } else {
                msg = i18np("Moved 1 old message from folder %2 to folder %3.",
                            "Moved %1 old messages from folder %2 to folder %3.",
                            mRemovedMsgs.count(), mSrcFolder.name(), mMoveToFolder.name());
            }
            break;

        case Akonadi::Job::UserCanceled:
            if (expirationAttribute->expireAction() == MailCommon::ExpireCollectionAttribute::ExpireDelete) {
                msg = i18n("Removing old messages from folder %1 was canceled.",
                           mSrcFolder.name());
            } else {
                msg = i18n("Moving old messages from folder %1 to folder %2 was "
                           "canceled.",
                           mSrcFolder.name(), mMoveToFolder.name());
            }
            break;

        default: //any other error
            if (expirationAttribute->expireAction() == MailCommon::ExpireCollectionAttribute::ExpireDelete) {
                msg = i18n("Removing old messages from folder %1 failed.",
                           mSrcFolder.name());
            } else {
                msg = i18n("Moving old messages from folder %1 to folder %2 failed.",
                           mSrcFolder.name(), mMoveToFolder.name());
            }
            break;
        }

        BroadcastStatus::instance()->setStatusMsg(msg);
    }
    deleteLater();
}
}
