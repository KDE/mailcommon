/*
 *
 *  SPDX-FileCopyrightText: 2003 Zack Rusin <zack@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef MAILCOMMON_FOLDERJOB_H
#define MAILCOMMON_FOLDERJOB_H

#include "mailcommon_export.h"
#include <Collection>

namespace MailCommon
{
/**
 * @brief The FolderJob class
 */
class MAILCOMMON_EXPORT FolderJob : public QObject
{
    Q_OBJECT

public:
    explicit FolderJob(QObject *parent = nullptr);

    ~FolderJob() override;

    /**
     * Start the job
     */
    void start();

    /**
     * Interrupt the job. Note that the finished() and result() signal
     * will be emitted, unless you called setPassiveDestructor(true) before.
     * This kills the job, don't use it afterwards.
     */
    virtual void kill();

    /**
     * @return the error code of the job. This must only be called from
     * the slot connected to the finished() signal.
     */
    Q_REQUIRED_RESULT int error() const;

    /**
     * @return true if this job can be canceled, e.g. to exit the application
     */
    Q_REQUIRED_RESULT bool isCancellable() const;

    /**
     * Call this to change the "cancellable" property of this job.
     * By default, tListMessages, tGetMessage, tGetFolder and tCheckUidValidity
     * are cancellable, the others are not. But when copying, a non-cancellable
     * tGetMessage is needed.
     */
    void setCancellable(bool b);

Q_SIGNALS:
    /**
     * Emitted when the job finishes all processing.
     */
    void finished();

    /**
     * Emitted when the job finishes all processing.
     * More convenient signal than finished(), since it provides a pointer to the job.
     * This signal is emitted by the FolderJob destructor => do NOT downcast
     * the job to a subclass!
     */
    void result(FolderJob *job);

protected:
    /**
     * Has to be reimplemented. It's called by the start() method. Should
     * start the processing of the specified job function.
     */
    virtual void execute() = 0;

    Akonadi::Collection mSrcFolder;
    int mErrorCode = 0;

    bool mStarted = false;
    bool mCancellable = false;
};
}

#endif
