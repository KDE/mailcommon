/*

  SPDX-FileCopyrightText: 2004 Ingo Kloecker <kloecker@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "rulewidgethandlermanager.h"
#include "daterulewidgethandler.h"
#include "encryptionwidgethandler.h"
#include "headersrulerwidgethandler.h"
#include "interfaces/rulewidgethandler.h"
#include "messagerulewidgethandler.h"
#include "numericdoublerulewidgethandler.h"
#include "numericrulewidgethandler.h"
#include "statusrulewidgethandler.h"
#include "tagrulewidgethandler.h"
#include "textrulerwidgethandler.h"

#include <MessageViewer/Stl_Util>

#include <QObject>
#include <QStackedWidget>

#include <algorithm>
using std::for_each;
using std::remove;

using namespace MailCommon;

MailCommon::RuleWidgetHandlerManager *MailCommon::RuleWidgetHandlerManager::self = nullptr;

MailCommon::RuleWidgetHandlerManager::RuleWidgetHandlerManager()
{
    registerHandler(new MailCommon::TagRuleWidgetHandler());
    registerHandler(new MailCommon::DateRuleWidgetHandler());
    registerHandler(new MailCommon::NumericRuleWidgetHandler());
    registerHandler(new MailCommon::StatusRuleWidgetHandler());
    registerHandler(new MailCommon::MessageRuleWidgetHandler());
    registerHandler(new MailCommon::NumericDoubleRuleWidgetHandler());
    registerHandler(new MailCommon::HeadersRuleWidgetHandler());
    registerHandler(new MailCommon::EncryptionWidgetHandler());
    // the TextRuleWidgetHandler is the fallback handler, so it has to be added
    // as last handler
    registerHandler(new MailCommon::TextRuleWidgetHandler());
}

MailCommon::RuleWidgetHandlerManager::~RuleWidgetHandlerManager()
{
    for_each(mHandlers.begin(), mHandlers.end(), MessageViewer::DeleteAndSetToZero<RuleWidgetHandler>());
}

void MailCommon::RuleWidgetHandlerManager::setIsAkonadiSearch(bool isBalooSearch)
{
    mIsBalooSearch = isBalooSearch;
}

void MailCommon::RuleWidgetHandlerManager::registerHandler(const RuleWidgetHandler *handler)
{
    if (!handler) {
        return;
    }
    unregisterHandler(handler); // don't produce duplicates
    mHandlers.push_back(handler);
}

void MailCommon::RuleWidgetHandlerManager::unregisterHandler(const RuleWidgetHandler *handler)
{
    // don't delete them, only remove them from the list!
    mHandlers.erase(remove(mHandlers.begin(), mHandlers.end(), handler), mHandlers.end());
}

namespace
{
/**
 * Returns the number of immediate children of parent with the given object name.
 * Used by RuleWidgetHandlerManager::createWidgets().
 */
int childCount(const QObject *parent, const QString &objName)
{
    const QObjectList list = parent->children();
    int count = 0;
    for (QObject *item : list) {
        if (item->objectName() == objName) {
            count++;
        }
    }
    return count;
}
}

void MailCommon::RuleWidgetHandlerManager::createWidgets(QStackedWidget *functionStack, QStackedWidget *valueStack, const QObject *receiver) const
{
    const_iterator end(mHandlers.constEnd());
    for (const_iterator it = mHandlers.constBegin(); it != end; ++it) {
        QWidget *w = nullptr;
        for (int i = 0; (w = (*it)->createFunctionWidget(i, functionStack, receiver, mIsBalooSearch)); ++i) {
            if (childCount(functionStack, w->objectName()) < 2) {
                // there wasn't already a widget with this name, so add this widget
                functionStack->addWidget(w);
            } else {
                // there was already a widget with this name, so discard this widget
                delete w;
                w = nullptr;
            }
        }
        for (int i = 0; (w = (*it)->createValueWidget(i, valueStack, receiver)); ++i) {
            if (childCount(valueStack, w->objectName()) < 2) {
                // there wasn't already a widget with this name, so add this widget
                valueStack->addWidget(w);
            } else {
                // there was already a widget with this name, so discard this widget
                delete w;
                w = nullptr;
            }
        }
    }
}

SearchRule::Function MailCommon::RuleWidgetHandlerManager::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    const_iterator end(mHandlers.constEnd());
    for (const_iterator it = mHandlers.constBegin(); it != end; ++it) {
        const SearchRule::Function func = (*it)->function(field, functionStack);
        if (func != SearchRule::FuncNone) {
            return func;
        }
    }
    return SearchRule::FuncNone;
}

QString MailCommon::RuleWidgetHandlerManager::value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    const_iterator end(mHandlers.constEnd());
    for (const_iterator it = mHandlers.constBegin(); it != end; ++it) {
        const QString val = (*it)->value(field, functionStack, valueStack);
        if (!val.isEmpty()) {
            return val;
        }
    }
    return QString();
}

QString MailCommon::RuleWidgetHandlerManager::prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    const_iterator end(mHandlers.constEnd());
    for (const_iterator it = mHandlers.constBegin(); it != end; ++it) {
        const QString val = (*it)->prettyValue(field, functionStack, valueStack);
        if (!val.isEmpty()) {
            return val;
        }
    }
    return QString();
}

void MailCommon::RuleWidgetHandlerManager::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    const_iterator end(mHandlers.constEnd());
    for (const_iterator it = mHandlers.constBegin(); it != end; ++it) {
        (*it)->reset(functionStack, valueStack);
    }
    update("", functionStack, valueStack);
}

void MailCommon::RuleWidgetHandlerManager::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule) const
{
    Q_ASSERT(rule);
    reset(functionStack, valueStack);
    const_iterator end(mHandlers.constEnd());
    for (const_iterator it = mHandlers.constBegin(); it != end; ++it) {
        if ((*it)->setRule(functionStack, valueStack, rule, mIsBalooSearch)) {
            return;
        }
    }
}

void MailCommon::RuleWidgetHandlerManager::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    const_iterator end(mHandlers.constEnd());
    for (const_iterator it = mHandlers.constBegin(); it != end; ++it) {
        if ((*it)->update(field, functionStack, valueStack)) {
            return;
        }
    }
}
