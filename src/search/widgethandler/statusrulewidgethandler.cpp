/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "statusrulewidgethandler.h"
#include "search/searchrule/searchrulestatus.h"

#include <QComboBox>
#include <QIcon>
#include <QStackedWidget>
#include <KLazyLocalizedString>
using namespace MailCommon;

static const struct {
    SearchRule::Function id;
    const KLazyLocalizedString displayName;
} StatusFunctions[] = {{SearchRule::FuncContains, kli18n("is")}, {SearchRule::FuncContainsNot, kli18n("is not")}};
static const int StatusFunctionCount = sizeof(StatusFunctions) / sizeof(*StatusFunctions);

struct MessageStatusInfo {
    const KLazyLocalizedString text;
    const char *icon;
};

// If you change the ordering here; also do it in the enum below
static const MessageStatusInfo StatusValues[] = {
    {kli18nc("message status", "Important"), "emblem-important"},
    {kli18nc("message status", "Action Item"), "mail-task"},
    {kli18nc("message status", "Unread"), "mail-unread"},
    {kli18nc("message status", "Read"), "mail-read"},
    {kli18nc("message status", "Deleted"), "mail-deleted"},
    {kli18nc("message status", "Replied"), "mail-replied"},
    {kli18nc("message status", "Forwarded"), "mail-forwarded"},
    {kli18nc("message status", "Queued"), "mail-queued"},
    {kli18nc("message status", "Sent"), "mail-sent"},
    {kli18nc("message status", "Watched"), "mail-thread-watch"},
    {kli18nc("message status", "Ignored"), "mail-thread-ignored"},
    {kli18nc("message status", "Spam"), "mail-mark-junk"},
    {kli18nc("message status", "Ham"), "mail-mark-notjunk"},
    {kli18nc("message status", "Has Attachment"), "mail-attachment"} // must be last
};

static const int StatusValueCount = sizeof(StatusValues) / sizeof(MessageStatusInfo);
// we want to show all status entries in the quick search bar, but only the
// ones up to attachment in the search/filter dialog, because there the
// attachment case is handled separately.
static const int StatusValueCountWithoutHidden = StatusValueCount - 1;

//---------------------------------------------------------------------------

QWidget *StatusRuleWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool /*isBalooSearch*/) const
{
    if (number != 0) {
        return nullptr;
    }

    auto funcCombo = new QComboBox(functionStack);
    funcCombo->setMinimumWidth(50);
    funcCombo->setObjectName(QStringLiteral("statusRuleFuncCombo"));
    for (int i = 0; i < StatusFunctionCount; ++i) {
        funcCombo->addItem(StatusFunctions[i].displayName.toString());
    }
    funcCombo->adjustSize();
    QObject::connect(funcCombo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return funcCombo;
}

//---------------------------------------------------------------------------

QWidget *StatusRuleWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
{
    if (number != 0) {
        return nullptr;
    }

    auto statusCombo = new QComboBox(valueStack);
    statusCombo->setMinimumWidth(50);
    statusCombo->setObjectName(QStringLiteral("statusRuleValueCombo"));
    for (int i = 0; i < StatusValueCountWithoutHidden; ++i) {
        if (StatusValues[i].icon != nullptr) {
            statusCombo->addItem(QIcon::fromTheme(QLatin1String(StatusValues[i].icon)), StatusValues[i].text.toString());
        } else {
            statusCombo->addItem(StatusValues[i].text.toString());
        }
    }
    statusCombo->adjustSize();
    QObject::connect(statusCombo, SIGNAL(activated(int)), receiver, SLOT(slotValueChanged()));
    return statusCombo;
}

//---------------------------------------------------------------------------

SearchRule::Function StatusRuleWidgetHandler::currentFunction(const QStackedWidget *functionStack) const
{
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("statusRuleFuncCombo"));

    if (funcCombo && funcCombo->currentIndex() >= 0) {
        return StatusFunctions[funcCombo->currentIndex()].id;
    }

    return SearchRule::FuncNone;
}

//---------------------------------------------------------------------------

SearchRule::Function StatusRuleWidgetHandler::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    if (!handlesField(field)) {
        return SearchRule::FuncNone;
    }

    return currentFunction(functionStack);
}

//---------------------------------------------------------------------------

int StatusRuleWidgetHandler::currentStatusValue(const QStackedWidget *valueStack) const
{
    const auto statusCombo = valueStack->findChild<QComboBox *>(QStringLiteral("statusRuleValueCombo"));

    if (statusCombo) {
        return statusCombo->currentIndex();
    }

    return -1;
}

//---------------------------------------------------------------------------

QString StatusRuleWidgetHandler::value(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return {};
    }

    const int status = currentStatusValue(valueStack);
    if (status != -1) {
        return StatusValues[status].text.untranslatedText();
    } else {
        return {};
    }
}

//---------------------------------------------------------------------------

QString StatusRuleWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return {};
    }

    const int status = currentStatusValue(valueStack);
    if (status != -1) {
        return StatusValues[status].text.toString();
    } else {
        return {};
    }
}

//---------------------------------------------------------------------------

bool StatusRuleWidgetHandler::handlesField(const QByteArray &field) const
{
    return field == "<status>";
}

//---------------------------------------------------------------------------

void StatusRuleWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    // reset the function combo box
    auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("statusRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        funcCombo->setCurrentIndex(0);
        funcCombo->blockSignals(false);
    }

    // reset the status value combo box
    auto statusCombo = valueStack->findChild<QComboBox *>(QStringLiteral("statusRuleValueCombo"));

    if (statusCombo) {
        statusCombo->blockSignals(true);
        statusCombo->setCurrentIndex(0);
        statusCombo->blockSignals(false);
    }
}

//---------------------------------------------------------------------------

bool StatusRuleWidgetHandler::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool /*isBalooSearch*/) const
{
    if (!rule || !handlesField(rule->field())) {
        reset(functionStack, valueStack);
        return false;
    }

    // set the function
    const SearchRule::Function func = rule->function();
    int funcIndex = 0;
    for (; funcIndex < StatusFunctionCount; ++funcIndex) {
        if (func == StatusFunctions[funcIndex].id) {
            break;
        }
    }

    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("statusRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        if (funcIndex < StatusFunctionCount) {
            funcCombo->setCurrentIndex(funcIndex);
        } else {
            funcCombo->setCurrentIndex(0);
        }
        funcCombo->blockSignals(false);
        functionStack->setCurrentWidget(funcCombo);
    }

    // set the value
    const QString value = rule->contents();
    int valueIndex = 0;
    for (; valueIndex < StatusValueCountWithoutHidden; ++valueIndex) {
        if (value == StatusValues[valueIndex].text.untranslatedText()) {
            break;
        }
    }

    const auto statusCombo = valueStack->findChild<QComboBox *>(QStringLiteral("statusRuleValueCombo"));

    if (statusCombo) {
        statusCombo->blockSignals(true);
        if (valueIndex < StatusValueCountWithoutHidden) {
            statusCombo->setCurrentIndex(valueIndex);
        } else {
            statusCombo->setCurrentIndex(0);
        }
        statusCombo->blockSignals(false);
        valueStack->setCurrentWidget(statusCombo);
    }
    return true;
}

//---------------------------------------------------------------------------

bool StatusRuleWidgetHandler::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return false;
    }

    // raise the correct function widget
    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("statusRuleFuncCombo")));

    // raise the correct value widget
    valueStack->setCurrentWidget(valueStack->findChild<QWidget *>(QStringLiteral("statusRuleValueCombo")));

    return true;
}
