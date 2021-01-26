/*
 * SPDX-FileCopyrightText: 2004 David Faure <faure@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef MAILCOMMON_JOBSCHEDULER_H
#define MAILCOMMON_JOBSCHEDULER_H

#include "mailcommon_export.h"

#include <QObject>

#include <QTimer>
#include <QVector>

#include "folderjob.h"
#include <collection.h>
// If this define is set, JobScheduler will show debug output, and related kmkernel
// timers will be shortened.
// This is for debugging purposes only, don't commit with it.
//#define DEBUG_SCHEDULER

namespace MailCommon
{
class FolderJob;
class ScheduledJob;

/**
 * A scheduled task is some information about a folder job that should be run later.
 * As long as it's not running, it's called a "task", i.e. something that needs to be done.
 * Tasks are managed by the JobScheduler.
 */
class MAILCOMMON_EXPORT ScheduledTask
{
public:
    /**
     * Creates a scheduled task for a given folder.
     * If @p immediate is true, the scheduler will run this task as soon
     * as possible (but won't interrupt a currently running job for it).
     */
    ScheduledTask(const Akonadi::Collection &folder, bool immediate);
    virtual ~ScheduledTask();

    /**
     * Run this task, i.e. create a job for it.
     * Important: the job's execute() method must either call open() on the folder
     * or storage immediately, or abort (deleting itself).
     * Usually, that job should also be cancellable.
     * Otherwise (if the open() is delayed) an unrelated open() could happen first
     * and mess things up.
     * If for some reason (e.g. a folder is deleted) nothing should be done, return 0.
     */
    virtual ScheduledJob *run() = 0;

    /**
     * An identifier for the type of task (a bit like QListViewItem::rtti).
     * This allows to automatically prevent two identical tasks from being scheduled
     * for the same folder. To circumvent this feature and make every task unique,
     * return 0 here.
     */
    virtual int taskTypeId() const = 0;

    /**
     * The folder which this task is supposed to handle, 0 if it was deleted meanwhile.
     */
    Q_REQUIRED_RESULT Akonadi::Collection folder() const
    {
        return mCurrentFolder;
    }

    Q_REQUIRED_RESULT bool isImmediate() const
    {
        return mImmediate;
    }

private:
    Akonadi::Collection mCurrentFolder;
    bool mImmediate;
};

/**
 * The unique JobScheduler instance (owned by kmkernel) implements "background processing"
 * of folder operations (like expiration and compaction). Tasks (things to be done)
 * are registered with the JobScheduler, and it will execute them one at a time,
 * separated by a 1-minute timer. The jobs themselves should use timers to avoid
 * using too much CPU for too long. Tasks for opened folders are not executed until
 * the folder is closed.
 */
class MAILCOMMON_EXPORT JobScheduler : public QObject
{
    Q_OBJECT
public:
    explicit JobScheduler(QObject *parent);
    ~JobScheduler();

    /**
     * Register a task to be done for a given folder. The ownership of the task is transferred
     * to the JobScheduler.
     */
    void registerTask(ScheduledTask *task);

    // D-Bus calls, called from KMKernel
    void pause();
    void resume();

private:
    // Called by a timer to run the next job
    void slotRunNextJob();

    // Called when the current job terminates
    void slotJobFinished();
    void restartTimer();
    void interruptCurrentTask();
    void runTaskNow(ScheduledTask *task);
    using TaskList = QVector<ScheduledTask *>;
    void removeTask(TaskList::Iterator &it);

private:
    TaskList mTaskList; // FIFO of tasks to be run

    QTimer mTimer;
    int mPendingImmediateTasks = 0;

    /// Information about the currently running job, if any
    ScheduledTask *mCurrentTask = nullptr;
    ScheduledJob *mCurrentJob = nullptr;
};

/**
 * Base class for scheduled jobs.
 */
class MAILCOMMON_EXPORT ScheduledJob : public FolderJob
{
public:
    ScheduledJob(const Akonadi::Collection &folder, bool immediate);
    ~ScheduledJob();

protected:
    bool mImmediate;
};
}

#endif
