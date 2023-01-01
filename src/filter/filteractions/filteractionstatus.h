/*

  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include "filteractionwithstringlist.h"
#include <Akonadi/MessageStatus>

namespace MailCommon
{
class FilterActionStatus : public FilterActionWithStringList
{
    Q_OBJECT
public:
    FilterActionStatus(const QString &name, const QString &label, QObject *parent = nullptr);
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    Q_REQUIRED_RESULT bool isEmpty() const override;
    void argsFromString(const QString &argsStr) override;
    Q_REQUIRED_RESULT QString argsAsString() const override;
    Q_REQUIRED_RESULT QString displayString() const override;

    static QString realStatusString(const QString &statusStr);
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;

    static const Akonadi::MessageStatus statuses[];
    static const int StatiCount;
};
}
