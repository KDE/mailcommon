/*
 * SPDX-FileCopyrightText: 2004 David Faure <faure@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "mailcommon_export.h"

#include <QObject>

#include <QList>
#include <QTimer>

#include "folderjob.h"
#include <Akonadi/Collection>
// If this define is set, JobScheduler will show debug output, and related kmkernel
// timers will be shortened.
// This is for debugging purposes only, don't commit with it.
// #define DEBUG_SCHEDULER

namespace MailCommon
{
class FolderJob;
class ScheduledJob;

/*!
 * \class MailCommon::ScheduledTask
 * \inmodule MailCommon
 * \inheaderfile MailCommon/JobScheduler
 *
 * A scheduled task is some information about a folder job that should be run later.
 * As long as it's not running, it's called a "task", i.e. something that needs to be done.
 * Tasks are managed by the JobScheduler.
 */
class MAILCOMMON_EXPORT ScheduledTask
{
public:
    /*!
     * Creates a scheduled task for a given folder.
     * If \a immediate is true, the scheduler will run this task as soon
     * as possible (but won't interrupt a currently running job for it).
     *
     * \param folder The Akonadi collection to perform the task on
     * \param immediate Whether the task should be run immediately
     */
    ScheduledTask(const Akonadi::Collection &folder, bool immediate);
    /*!
     * Destroys the scheduled task.
     */
    virtual ~ScheduledTask();

    /*!
     * Run this task, i.e. create a job for it.
     * Important: the job's execute() method must either call open() on the folder
     * or storage immediately, or abort (deleting itself).
     * Usually, that job should also be cancellable.
     * Otherwise (if the open() is delayed) an unrelated open() could happen first
     * and mess things up.
     * If for some reason (e.g. a folder is deleted) nothing should be done, return 0.
     *
     * \return A ScheduledJob to execute, or nullptr if nothing should be done
     */
    virtual ScheduledJob *run() = 0;

    /*!
     * An identifier for the type of task (a bit like QListViewItem::rtti).
     * This allows to automatically prevent two identical tasks from being scheduled
     * for the same folder. To circumvent this feature and make every task unique,
     * return 0 here.
     *
     * \return A unique task type ID, or 0 for unique tasks
     */
    virtual int taskTypeId() const = 0;

    /*!
     * The folder which this task is supposed to handle, 0 if it was deleted meanwhile.
     *
     * \return The collection for this task
     */
    [[nodiscard]] Akonadi::Collection folder() const;

    /*!
     * Returns whether this task should be run immediately.
     *
     * \return True if the task is immediate, false otherwise
     */
    [[nodiscard]] bool isImmediate() const;

private:
    const Akonadi::Collection mCurrentFolder;
    const bool mImmediate;
};

/*!
 * \class MailCommon::ScheduledJob
 * \inmodule MailCommon
 * \inheaderfile MailCommon/JobScheduler
 *
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
    /*!
     * Constructs a job scheduler.
     *
     * \param parent The parent object
     */
    explicit JobScheduler(QObject *parent);
    /*!
     * Destroys the job scheduler.
     */
    ~JobScheduler() override;

    /*!
     * Register a task to be done for a given folder. The ownership of the task is transferred
     * to the JobScheduler.
     *
     * \param task The task to register
     */
    void registerTask(ScheduledTask *task);

    // D-Bus calls, called from KMKernel
    /*!
     * Pauses the job scheduler, preventing new jobs from being started.
     */
    void pause();
    /*!
     * Resumes the job scheduler after being paused.
     */
    void resume();

private:
    // Called by a timer to run the next job
    MAILCOMMON_NO_EXPORT void slotRunNextJob();

    // Called when the current job terminates
    MAILCOMMON_NO_EXPORT void slotJobFinished();
    MAILCOMMON_NO_EXPORT void restartTimer();
    MAILCOMMON_NO_EXPORT void interruptCurrentTask();
    MAILCOMMON_NO_EXPORT void runTaskNow(ScheduledTask *task);
    using TaskList = QList<ScheduledTask *>;
    MAILCOMMON_NO_EXPORT void removeTask(TaskList::Iterator &it);

private:
    TaskList mTaskList; // FIFO of tasks to be run

    QTimer mTimer;
    int mPendingImmediateTasks = 0;

    /// Information about the currently running job, if any
    ScheduledTask *mCurrentTask = nullptr;
    ScheduledJob *mCurrentJob = nullptr;
};

/*!
 * \class MailCommon::ScheduledJob
 * \inmodule MailCommon
 * \inheaderfile MailCommon/JobScheduler
 *
 * Base class for scheduled jobs.
 */
class MAILCOMMON_EXPORT ScheduledJob : public FolderJob
{
public:
    /*!
     * Constructs a scheduled job.
     *
     * \param folder The Akonadi collection to perform the job on
     * \param immediate Whether the job should be run immediately
     */
    ScheduledJob(const Akonadi::Collection &folder, bool immediate);
    /*!
     * Destroys the scheduled job.
     */
    ~ScheduledJob() override;

protected:
    /*!
     * Whether this job should be run immediately.
     */
    bool mImmediate;
};
}
