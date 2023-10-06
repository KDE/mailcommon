/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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
 * @brief The MDNWarningWidgetJob class
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
    [[nodiscard]] MAILCOMMON_NO_EXPORT QPair<bool, KMime::MDN::SendingMode> modifyItem(const KMime::Message::Ptr &msg);
    Akonadi::Item mItem;
    ResponseMDN mResponse = Unknown;
    KMime::MDN::SendingMode mSendingMode = KMime::MDN::SendingMode::SentAutomatically;
    IKernel *const mKernel = nullptr;
};
}
