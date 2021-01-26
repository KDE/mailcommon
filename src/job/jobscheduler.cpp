/**
 * SPDX-FileCopyrightText: 2004 David Faure <faure@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "jobscheduler.h"
#include "mailcommon_debug.h"
namespace MailCommon
{
ScheduledTask::ScheduledTask(const Akonadi::Collection &folder, bool immediate)
    : mCurrentFolder(folder)
    , mImmediate(immediate)
{
}

ScheduledTask::~ScheduledTask()
{
}

JobScheduler::JobScheduler(QObject *parent)
    : QObject(parent)
    , mTimer(this)
{
    connect(&mTimer, &QTimer::timeout, this, &JobScheduler::slotRunNextJob);
    // No need to start the internal timer yet, we wait for a task to be scheduled
}

JobScheduler::~JobScheduler()
{
    qDeleteAll(mTaskList);
    mTaskList.clear();
    delete mCurrentTask;
    mCurrentTask = nullptr;
    delete mCurrentJob;
}

void JobScheduler::registerTask(ScheduledTask *task)
{
    bool immediate = task->isImmediate();
    int typeId = task->taskTypeId();
    if (typeId) {
        const Akonadi::Collection folder = task->folder();
        // Search for an identical task already scheduled
        TaskList::Iterator end(mTaskList.end());
        for (TaskList::Iterator it = mTaskList.begin(); it != end; ++it) {
            if ((*it)->taskTypeId() == typeId && (*it)->folder() == folder) {
#ifdef DEBUG_SCHEDULER
                qCDebug(MAILCOMMON_LOG) << "JobScheduler: already having task type" << typeId << "for folder" << folder->label();
#endif
                delete task;
                if (!mCurrentTask && immediate) {
                    ScheduledTask *task = *it;
                    removeTask(it);
                    runTaskNow(task);
                }
                return;
            }
        }
        // Note that scheduling an identical task as the one currently running is allowed.
    }
    if (!mCurrentTask && immediate) {
        runTaskNow(task);
    } else {
#ifdef DEBUG_SCHEDULER
        qCDebug(MAILCOMMON_LOG) << "JobScheduler: adding task" << task << "(type" << task->taskTypeId() << ") for folder" << task->folder()
                                << task->folder().name();
#endif
        mTaskList.append(task);
        if (immediate) {
            ++mPendingImmediateTasks;
        }
        if (!mCurrentTask && !mTimer.isActive()) {
            restartTimer();
        }
    }
}

void JobScheduler::removeTask(TaskList::Iterator &it)
{
    if ((*it)->isImmediate()) {
        --mPendingImmediateTasks;
    }
    mTaskList.erase(it);
}

void JobScheduler::interruptCurrentTask()
{
    Q_ASSERT(mCurrentTask);
#ifdef DEBUG_SCHEDULER
    qCDebug(MAILCOMMON_LOG) << "JobScheduler: interrupting job" << mCurrentJob << "for folder" << mCurrentTask->folder()->label();
#endif
    // File it again. This will either delete it or put it in mTaskList.
    registerTask(mCurrentTask);
    mCurrentTask = nullptr;
    mCurrentJob->kill(); // This deletes the job and calls slotJobFinished!
}

void JobScheduler::slotRunNextJob()
{
    while (!mCurrentJob) {
#ifdef DEBUG_SCHEDULER
        qCDebug(MAILCOMMON_LOG) << "JobScheduler: slotRunNextJob";
#endif
        Q_ASSERT(mCurrentTask == nullptr);
        ScheduledTask *task = nullptr;
        // Find a task suitable for being run
        TaskList::Iterator end(mTaskList.end());
        for (TaskList::Iterator it = mTaskList.begin(); it != end; ++it) {
            // Remove if folder died
            const Akonadi::Collection folder = (*it)->folder();
            if (!folder.isValid()) {
#ifdef DEBUG_SCHEDULER
                qCDebug(MAILCOMMON_LOG) << "  folder for task" << (*it) << "was deleted";
#endif
                removeTask(it);
                if (!mTaskList.isEmpty()) {
                    slotRunNextJob(); // to avoid the mess with invalid iterators :)
                } else {
                    mTimer.stop();
                }
                return;
            }
#ifdef DEBUG_SCHEDULER
            qCDebug(MAILCOMMON_LOG) << "  looking at folder" << folder.name();
#endif
            task = *it;
            removeTask(it);
            break;
        }

        if (!task) { // found nothing to run, i.e. folder was opened
            return; // Timer keeps running, i.e. try again in 1 minute
        }

        runTaskNow(task);
    } // If nothing to do for that task, loop and find another one to run
}

void JobScheduler::restartTimer()
{
    if (mPendingImmediateTasks > 0) {
        slotRunNextJob();
    } else {
#ifdef DEBUG_SCHEDULER
        mTimer.start(10000); // 10 seconds
#else
        mTimer.start(1 * 60000); // 1 minute
#endif
    }
}

void JobScheduler::runTaskNow(ScheduledTask *task)
{
    Q_ASSERT(mCurrentTask == nullptr);
    if (mCurrentTask) {
        interruptCurrentTask();
    }
    mCurrentTask = task;
    mTimer.stop();
    mCurrentJob = mCurrentTask->run();
#ifdef DEBUG_SCHEDULER
    qCDebug(MAILCOMMON_LOG) << "JobScheduler: task" << mCurrentTask << "(type" << mCurrentTask->taskTypeId() << ")"
                            << "for folder" << mCurrentTask->folder()->label() << "returned job" << mCurrentJob << (mCurrentJob ? mCurrentJob->className() : 0);
#endif
    if (!mCurrentJob) { // nothing to do, e.g. folder deleted
        delete mCurrentTask;
        mCurrentTask = nullptr;
        if (!mTaskList.isEmpty()) {
            restartTimer();
        }
        return;
    }
    // Register the job in the folder. This makes it autodeleted if the folder is deleted.
#if 0
    mCurrentTask->folder()->storage()->addJob(mCurrentJob);
#endif
    connect(mCurrentJob, &ScheduledJob::finished, this, &JobScheduler::slotJobFinished);
    mCurrentJob->start();
}

void JobScheduler::slotJobFinished()
{
    // Do we need to test for mCurrentJob->error()? What do we do then?
#ifdef DEBUG_SCHEDULER
    qCDebug(MAILCOMMON_LOG) << "JobScheduler: slotJobFinished";
#endif
    delete mCurrentTask;
    mCurrentTask = nullptr;
    mCurrentJob = nullptr;
    if (!mTaskList.isEmpty()) {
        restartTimer();
    }
}

// D-Bus call to pause any background jobs
void JobScheduler::pause()
{
    mPendingImmediateTasks = 0;
    if (mCurrentJob && mCurrentJob->isCancellable()) {
        interruptCurrentTask();
    }
    mTimer.stop();
}

void JobScheduler::resume()
{
    restartTimer();
}

////

ScheduledJob::ScheduledJob(const Akonadi::Collection &folder, bool immediate)
    : mImmediate(immediate)
{
    mCancellable = true;
    mSrcFolder = folder;
}

ScheduledJob::~ScheduledJob()
{
}
}
