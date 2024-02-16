/*

  SPDX-FileCopyrightText: 2004 Ingo Kloecker <kloecker@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "search/searchpattern.h"

#include <QByteArray>
#include <QList>

class QObject;
class QString;
class QStackedWidget;

namespace MailCommon
{
class RuleWidgetHandler;

/**
 * @short Singleton to manage the list of RuleWidgetHandlers
 */
class RuleWidgetHandlerManager
{
    static RuleWidgetHandlerManager *self;

    RuleWidgetHandlerManager();

public:
    ~RuleWidgetHandlerManager();

    static RuleWidgetHandlerManager *instance()
    {
        if (!self) {
            self = new RuleWidgetHandlerManager();
        }
        return self;
    }

    void setIsAkonadiSearch(bool isBalooSearch);

    void registerHandler(const RuleWidgetHandler *handler);
    void unregisterHandler(const RuleWidgetHandler *handler);

    void createWidgets(QStackedWidget *functionStack, QStackedWidget *valueStack, const QObject *receiver) const;

    MailCommon::SearchRule::Function function(const QByteArray &field, const QStackedWidget *functionStack) const;

    [[nodiscard]] QString value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const;

    [[nodiscard]] QString prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const;

    void reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const;

    void setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const MailCommon::SearchRule::Ptr rule) const;

    void update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const;

private:
    using const_iterator = QList<const RuleWidgetHandler *>::const_iterator;
    using iterator = QList<const RuleWidgetHandler *>::iterator;

    QList<const RuleWidgetHandler *> mHandlers;
    bool mIsBalooSearch = false;
};
} // namespace MailCommon
