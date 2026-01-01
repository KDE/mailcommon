/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "interfaces/rulewidgethandler.h"

namespace MailCommon
{
class AttachmentWidgetHandler : public RuleWidgetHandler
{
public:
    AttachmentWidgetHandler();
    ~AttachmentWidgetHandler() override;

    [[nodiscard]] QWidget *createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isBalooSearch) const override;
    [[nodiscard]] QWidget *createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const override;
    [[nodiscard]] MailCommon::SearchRule::Function function(const QByteArray &field, const QStackedWidget *functionStack) const override;
    bool handlesField(const QByteArray &field) const override;
    [[nodiscard]] QString prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const override;
    bool setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const MailCommon::SearchRule::Ptr rule, bool isBalooSearch) const override;
    void reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const override;
    bool update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const override;
    [[nodiscard]] QString value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const override;
};
}
