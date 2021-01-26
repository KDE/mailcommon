/*

  SPDX-FileCopyrightText: 2004 Ingo Kloecker <kloecker@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_RULEWIDGETHANDLERMANAGER_H
#define MAILCOMMON_RULEWIDGETHANDLERMANAGER_H

#include "search/searchpattern.h"

#include <QByteArray>
#include <QVector>

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

    QString value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const;

    QString prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const;

    void reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const;

    void setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const MailCommon::SearchRule::Ptr rule) const;

    void update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const;

private:
    using const_iterator = QVector<const RuleWidgetHandler *>::const_iterator;
    using iterator = QVector<const RuleWidgetHandler *>::iterator;

    QVector<const RuleWidgetHandler *> mHandlers;
    bool mIsBalooSearch = false;
};
} // namespace MailCommon

#endif
