/*
  SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/Item>
#include <MailCommon/MailKernel>
#include <MessageComposer/MDNAdviceHelper>
#include <QObject>

namespace MailCommon
{
class IKernel;
/*!
 * \class MailCommon::MDNWarningJob
 * \inmodule MailCommon
 * \inheaderfile MailCommon/MDNWarningJob
 *
 * \brief The MDNWarningJob class
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT MDNWarningJob : public QObject
{
    Q_OBJECT
public:
    enum ResponseMDN {
        Unknown = 0,
        MDNIgnore,
        Denied,
        Send,
    };
    /*!
     * Constructs an MDN warning job.
     *
     * \param kernel The mail kernel object
     * \param parent The parent object
     */
    explicit MDNWarningJob(IKernel *kernel, QObject *parent = nullptr);
    /*!
     * Destroys the MDN warning job.
     */
    ~MDNWarningJob() override;

    /*!
     * Starts the MDN warning process.
     */
    void start();

    /*!
     * Returns the Akonadi item being processed.
     *
     * \return The Akonadi item
     */
    [[nodiscard]] const Akonadi::Item &item() const;
    /*!
     * Sets the Akonadi item to process.
     *
     * \param newItem The Akonadi item
     */
    void setItem(const Akonadi::Item &newItem);

    /*!
     * Checks if the job can be started.
     *
     * \return True if the job can start, false otherwise
     */
    [[nodiscard]] bool canStart() const;

    /*!
     * Returns the MDN response.
     *
     * \return The MDN response
     */
    [[nodiscard]] ResponseMDN response() const;
    /*!
     * Sets the MDN response.
     *
     * \param newResponse The MDN response to set
     */
    void setResponse(ResponseMDN newResponse);

    /*!
     * Returns the MDN sending mode.
     *
     * \return The sending mode
     */
    [[nodiscard]] KMime::MDN::SendingMode sendingMode() const;
    /*!
     * Sets the MDN sending mode.
     *
     * \param newSendingMode The sending mode to set
     */
    void setSendingMode(KMime::MDN::SendingMode newSendingMode);

Q_SIGNALS:
    /*!
     * Emitted when the MDN warning job has finished processing.
     */
    void finished();

private:
    struct MDNSendingInfo {
        KMime::MDN::SendingMode mode = KMime::MDN::SendingMode::SentAutomatically;
        bool doSend = false;
    };
    [[nodiscard]] MAILCOMMON_NO_EXPORT MDNSendingInfo modifyItem(const std::shared_ptr<KMime::Message> &msg);
    Akonadi::Item mItem;
    ResponseMDN mResponse = Unknown;
    KMime::MDN::SendingMode mSendingMode = KMime::MDN::SendingMode::SentAutomatically;
    IKernel *const mKernel = nullptr;
};
}
