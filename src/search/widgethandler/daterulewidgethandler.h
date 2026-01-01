/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "interfaces/rulewidgethandler.h"

namespace MailCommon
{
class DateRuleWidgetHandler : public MailCommon::RuleWidgetHandler
{
public:
    DateRuleWidgetHandler()
        : MailCommon::RuleWidgetHandler()
    {
    }

    ~DateRuleWidgetHandler() override = default;

    QWidget *createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isBalooSearch) const override;

    QWidget *createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const override;

    SearchRule::Function function(const QByteArray &field, const QStackedWidget *functionStack) const override;

    [[nodiscard]] QString value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const override;

    [[nodiscard]] QString prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const override;

    [[nodiscard]] bool handlesField(const QByteArray &field) const override;

    void reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const override;

    [[nodiscard]] bool setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool isBalooSearch) const override;

    bool update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const override;

private:
    [[nodiscard]] SearchRule::Function currentFunction(const QStackedWidget *functionStack) const;
    [[nodiscard]] QString currentValue(const QStackedWidget *valueStack) const;
};
}
