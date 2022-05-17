/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/Item>
#include <MailCommon/MailKernel>
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
    explicit MDNWarningJob(IKernel *kernel, QObject *parent = nullptr);
    ~MDNWarningJob() override;

    void start();

    Q_REQUIRED_RESULT const Akonadi::Item &item() const;
    void setItem(const Akonadi::Item &newItem);

    Q_REQUIRED_RESULT bool canStart() const;

Q_SIGNALS:
    void showWarning();

private:
    void modifyItem();
    Akonadi::Item mItem;
    IKernel *const mKernel = nullptr;
};
}
