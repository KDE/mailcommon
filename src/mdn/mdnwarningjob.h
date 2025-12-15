/*
  SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

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
/**
 * @brief The MDNWarningJob class
 * @author Laurent Montel <montel@kde.org>
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
    explicit MDNWarningJob(IKernel *kernel, QObject *parent = nullptr);
    ~MDNWarningJob() override;

    void start();

    [[nodiscard]] const Akonadi::Item &item() const;
    void setItem(const Akonadi::Item &newItem);

    [[nodiscard]] bool canStart() const;

    [[nodiscard]] ResponseMDN response() const;
    void setResponse(ResponseMDN newResponse);

    [[nodiscard]] KMime::MDN::SendingMode sendingMode() const;
    void setSendingMode(KMime::MDN::SendingMode newSendingMode);

Q_SIGNALS:
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
