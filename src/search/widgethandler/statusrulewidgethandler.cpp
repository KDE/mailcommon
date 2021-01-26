/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "statusrulewidgethandler.h"
#include "search/searchrule/searchrulestatus.h"

#include <QComboBox>
#include <QIcon>
#include <QStackedWidget>

using namespace MailCommon;

static const struct {
    SearchRule::Function id;
    const char *displayName;
} StatusFunctions[] = {{SearchRule::FuncContains, I18N_NOOP("is")}, {SearchRule::FuncContainsNot, I18N_NOOP("is not")}};
static const int StatusFunctionCount = sizeof(StatusFunctions) / sizeof(*StatusFunctions);

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
        funcCombo->addItem(i18n(StatusFunctions[i].displayName));
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
    for (int i = 0; i < MailCommon::StatusValueCountWithoutHidden; ++i) {
        if (MailCommon::StatusValues[i].icon != nullptr) {
            statusCombo->addItem(QIcon::fromTheme(QLatin1String(MailCommon::StatusValues[i].icon)), i18nc("message status", MailCommon::StatusValues[i].text));
        } else {
            statusCombo->addItem(i18nc("message status", MailCommon::StatusValues[i].text));
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
        return QString();
    }

    const int status = currentStatusValue(valueStack);
    if (status != -1) {
        return QString::fromLatin1(MailCommon::StatusValues[status].text);
    } else {
        return QString();
    }
}

//---------------------------------------------------------------------------

QString StatusRuleWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return QString();
    }

    const int status = currentStatusValue(valueStack);
    if (status != -1) {
        return i18nc("message status", MailCommon::StatusValues[status].text);
    } else {
        return QString();
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
    for (; valueIndex < MailCommon::StatusValueCountWithoutHidden; ++valueIndex) {
        if (value == QString::fromLatin1(MailCommon::StatusValues[valueIndex].text)) {
            break;
        }
    }

    const auto statusCombo = valueStack->findChild<QComboBox *>(QStringLiteral("statusRuleValueCombo"));

    if (statusCombo) {
        statusCombo->blockSignals(true);
        if (valueIndex < MailCommon::StatusValueCountWithoutHidden) {
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
