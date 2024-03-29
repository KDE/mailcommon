/*  -*- mode: C++; c-file-style: "gnu" -*-

  SPDX-FileCopyrightText: 2004 Ingo Kloecker <kloecker@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "search/searchpattern.h"

#include <QByteArray>

class QWidget;
class QStackedWidget;
class QString;
class QObject;

namespace MailCommon
{
/**
 * @short An interface to filter/search rule widget handlers
 */
class RuleWidgetHandler
{
public:
    virtual ~RuleWidgetHandler() = default;

    [[nodiscard]] virtual QWidget *createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isAkonadiSearch) const = 0;
    [[nodiscard]] virtual QWidget *createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const = 0;
    [[nodiscard]] virtual MailCommon::SearchRule::Function function(const QByteArray &field, const QStackedWidget *functionStack) const = 0;
    [[nodiscard]] virtual QString value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const = 0;
    [[nodiscard]] virtual QString prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const = 0;
    [[nodiscard]] virtual bool handlesField(const QByteArray &field) const = 0;
    virtual void reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const = 0;
    [[nodiscard]] virtual bool
    setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const MailCommon::SearchRule::Ptr rule, bool isAkonadiSearch) const = 0;
    [[nodiscard]] virtual bool update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const = 0;
};
}
