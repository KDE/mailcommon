/*
  Copyright (c) 2017  Daniel Vrátil <dvratil@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAILCOMMON_SEARCH_ENCRYPTIONWIDGETHANDLER_H_
#define MAILCOMMON_SEARCH_ENCRYPTIONWIDGETHANDLER_H_

#include "interfaces/rulewidgethandler.h"

namespace MailCommon {
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
