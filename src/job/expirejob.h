/* -*- mode: C++ -*-
 * SPDX-FileCopyrightText: 2004 David Faure <faure@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef MAILCOMMON_EXPIREJOB_H
#define MAILCOMMON_EXPIREJOB_H

#include "jobscheduler.h"

#include <Collection>
#include <Item>

class KJob;

namespace MailCommon
{
class ExpireJob : public ScheduledJob
{
    Q_OBJECT
public:
    explicit ExpireJob(const Akonadi::Collection &folder, bool immediate);
    ~ExpireJob() override;

    void execute() override;
    void kill() override;

private:
    void slotDoWork();
    void slotExpireDone(KJob *job);
    void slotMoveDone(KJob *job);
    void itemFetchResult(KJob *job);
    void done();

private:
    Akonadi::Item::List mRemovedMsgs;
    qint64 mMaxUnreadTime = 0;
    qint64 mMaxReadTime = 0;
    bool mExpireMessagesWithoutInvalidDate = false;
    Akonadi::Collection mMoveToFolder;
};

/// A scheduled "expire mails in this folder" task.
class ScheduledExpireTask : public ScheduledTask
{
public:
    /// If immediate is set, the job will execute synchronously. This is used when
    /// the user requests explicitly that the operation should happen immediately.
    ScheduledExpireTask(const Akonadi::Collection &folder, bool immediate)
        : ScheduledTask(folder, immediate)
    {
    }

    ~ScheduledExpireTask() override
    {
    }

    ScheduledJob *run() override
    {
        return folder().isValid() ? new ExpireJob(folder(), isImmediate()) : nullptr;
    }

    int taskTypeId() const override
    {
        return 1;
    }
};
} // namespace

#endif
