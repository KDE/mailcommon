/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MESSAGERULEWIDGETHANDLER_H
#define MESSAGERULEWIDGETHANDLER_H

#include "interfaces/rulewidgethandler.h"

namespace MailCommon
{
class MessageRuleWidgetHandler : public MailCommon::RuleWidgetHandler
{
public:
    MessageRuleWidgetHandler()
        : MailCommon::RuleWidgetHandler()
    {
    }

    ~MessageRuleWidgetHandler() override
    {
    }

    QWidget *createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isBalooSearch) const override;

    QWidget *createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const override;

    SearchRule::Function function(const QByteArray &field, const QStackedWidget *functionStack) const override;

    QString value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const override;

    QString prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const override;

    bool handlesField(const QByteArray &field) const override;

    void reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const override;

    bool setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool isBalooSearch) const override;

    bool update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const override;

private:
    SearchRule::Function currentFunction(const QStackedWidget *functionStack) const;
    QString currentValue(const QStackedWidget *valueStack, SearchRule::Function func) const;
};
}

#endif // MESSAGERULEWIDGETHANDLER_H
