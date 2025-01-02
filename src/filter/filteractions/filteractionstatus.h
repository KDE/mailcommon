/*

  SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    [[nodiscard]] bool isEmpty() const override;
    void argsFromString(const QString &argsStr) override;
    [[nodiscard]] QString argsAsString() const override;
    [[nodiscard]] QString displayString() const override;

    static QString realStatusString(const QString &statusStr);
    [[nodiscard]] QString informationAboutNotValidAction() const override;

    static const Akonadi::MessageStatus statuses[];
    static const int StatiCount;
};
}
