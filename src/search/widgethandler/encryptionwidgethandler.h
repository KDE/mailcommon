/*
  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_SEARCH_ENCRYPTIONWIDGETHANDLER_H_
#define MAILCOMMON_SEARCH_ENCRYPTIONWIDGETHANDLER_H_

#include "interfaces/rulewidgethandler.h"

namespace MailCommon
{
class EncryptionWidgetHandler : public RuleWidgetHandler
{
public:
    explicit EncryptionWidgetHandler();
    ~EncryptionWidgetHandler() override;

    QWidget *createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isBalooSearch) const override;
    QWidget *createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const override;
    MailCommon::SearchRule::Function function(const QByteArray &field, const QStackedWidget *functionStack) const override;
    bool handlesField(const QByteArray &field) const override;
    QString prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const override;
    bool setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const MailCommon::SearchRule::Ptr rule, bool isBalooSearch) const override;
    void reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const override;
    bool update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const override;
    QString value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const override;
};
}

#endif
