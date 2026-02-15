/*
 *
 *  SPDX-FileCopyrightText: 2003 Zack Rusin <zack@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/Collection>
#include <QObject>
namespace MailCommon
{
/*!
 * \class MailCommon::FolderJob
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FolderJob
 *
 * \brief The FolderJob class
 */
class MAILCOMMON_EXPORT FolderJob : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs a folder job.
     *
     * \param parent The parent object
     */
    explicit FolderJob(QObject *parent = nullptr);

    /*!
     * Destroys the folder job.
     */
    ~FolderJob() override;

    /*!
     * Start the job
     */
    void start();

    /*!
     * Interrupt the job. Note that the finished() and result() signal
     * will be emitted, unless you called setPassiveDestructor(true) before.
     * This kills the job, don't use it afterwards.
     */
    virtual void kill();

    /*!
     * Returns the error code of the job. This must only be called from
     * the slot connected to the finished() signal.
     *
     * \return The error code
     */
    [[nodiscard]] int error() const;

    /*!
     * Returns true if this job can be canceled, e.g. to exit the application
     *
     * \return True if the job is cancellable, false otherwise
     */
    [[nodiscard]] bool isCancellable() const;

    /*!
     * Call this to change the "cancellable" property of this job.
     * By default, tListMessages, tGetMessage, tGetFolder and tCheckUidValidity
     * are cancellable, the others are not. But when copying, a non-cancellable
     * tGetMessage is needed.
     *
     * \param b Whether the job should be cancellable
     */
    void setCancellable(bool b);

Q_SIGNALS:
    /*!
     * Emitted when the job finishes all processing.
     */
    void finished();

    /*!
     * Emitted when the job finishes all processing.
     * More convenient signal than finished(), since it provides a pointer to the job.
     * This signal is emitted by the FolderJob destructor => do NOT downcast
     * the job to a subclass!
     *
     * \param job Pointer to this job
     */
    void result(MailCommon::FolderJob *job);

protected:
    /*!
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
