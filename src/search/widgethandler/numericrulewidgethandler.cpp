/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "numericrulewidgethandler.h"
#include "search/searchpattern.h"

#include <KLazyLocalizedString>
#include <KLocalization>
#include <KLocalizedString>
#include <QComboBox>
#include <QSpinBox>
#include <QStackedWidget>
using namespace MailCommon;

static const struct {
    SearchRule::Function id;
    const KLazyLocalizedString displayName;
} NumericFunctions[] = {{SearchRule::FuncEquals, kli18n("is equal to")},
                        {SearchRule::FuncNotEqual, kli18n("is not equal to")},
                        {SearchRule::FuncIsGreater, kli18n("is greater than")},
                        {SearchRule::FuncIsLessOrEqual, kli18n("is less than or equal to")},
                        {SearchRule::FuncIsLess, kli18n("is less than")},
                        {SearchRule::FuncIsGreaterOrEqual, kli18n("is greater than or equal to")}};
static const int NumericFunctionCount = sizeof(NumericFunctions) / sizeof(*NumericFunctions);

//---------------------------------------------------------------------------

QWidget *NumericRuleWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool /*isBalooSearch*/) const
{
    if (number != 0) {
        return nullptr;
    }

    auto funcCombo = new QComboBox(functionStack);
    funcCombo->setMinimumWidth(50);
    funcCombo->setObjectName(QLatin1StringView("numericRuleFuncCombo"));
    for (int i = 0; i < NumericFunctionCount; ++i) {
        funcCombo->addItem(NumericFunctions[i].displayName.toString());
    }
    funcCombo->adjustSize();
    QObject::connect(funcCombo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return funcCombo;
}

//---------------------------------------------------------------------------

QWidget *NumericRuleWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
{
    if (number != 0) {
        return nullptr;
    }

    auto numInput = new QSpinBox(valueStack);
    numInput->setObjectName(QLatin1StringView("QSpinBox"));
    QObject::connect(numInput, SIGNAL(valueChanged(int)), receiver, SLOT(slotValueChanged()));
    return numInput;
}

//---------------------------------------------------------------------------

SearchRule::Function NumericRuleWidgetHandler::currentFunction(const QStackedWidget *functionStack) const
{
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("numericRuleFuncCombo"));

    if (funcCombo && funcCombo->currentIndex() >= 0) {
        return NumericFunctions[funcCombo->currentIndex()].id;
    }

    return SearchRule::FuncNone;
}

//---------------------------------------------------------------------------

SearchRule::Function NumericRuleWidgetHandler::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    if (!handlesField(field)) {
        return SearchRule::FuncNone;
    }

    return currentFunction(functionStack);
}

//---------------------------------------------------------------------------

QString NumericRuleWidgetHandler::currentValue(const QStackedWidget *valueStack) const
{
    const QSpinBox *numInput = valueStack->findChild<QSpinBox *>(QStringLiteral("QSpinBox"));

    if (numInput) {
        return QString::number(numInput->value());
    }

    return {};
}

//---------------------------------------------------------------------------

QString NumericRuleWidgetHandler::value(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return {};
    }

    return currentValue(valueStack);
}

//---------------------------------------------------------------------------

QString NumericRuleWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return {};
    }

    return currentValue(valueStack);
}

//---------------------------------------------------------------------------

bool NumericRuleWidgetHandler::handlesField(const QByteArray &field) const
{
    return field == "<age in days>";
}

//---------------------------------------------------------------------------

void NumericRuleWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    // reset the function combo box
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("numericRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        funcCombo->setCurrentIndex(0);
        funcCombo->blockSignals(false);
    }

    // reset the value widget
    auto numInput = valueStack->findChild<QSpinBox *>(QStringLiteral("QSpinBox"));

    if (numInput) {
        numInput->blockSignals(true);
        numInput->setValue(0);
        numInput->blockSignals(false);
    }
}

//---------------------------------------------------------------------------

static void initNumInput(QSpinBox *numInput, const QByteArray &field)
{
    if (field == "<age in days>") {
        numInput->setMinimum(-10000);
        KLocalization::setupSpinBoxFormatString(numInput, ki18ncp("Unit suffix where units are days.", "%v day", "%v days"));
    }
}

//---------------------------------------------------------------------------

bool NumericRuleWidgetHandler::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool /*isBalooSearch*/) const
{
    if (!rule || !handlesField(rule->field())) {
        reset(functionStack, valueStack);
        return false;
    }

    // set the function
    const SearchRule::Function func = rule->function();
    int funcIndex = 0;
    for (; funcIndex < NumericFunctionCount; ++funcIndex) {
        if (func == NumericFunctions[funcIndex].id) {
            break;
        }
    }

    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("numericRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        if (funcIndex < NumericFunctionCount) {
            funcCombo->setCurrentIndex(funcIndex);
        } else {
            funcCombo->setCurrentIndex(0);
        }
        funcCombo->blockSignals(false);
        functionStack->setCurrentWidget(funcCombo);
    }

    // set the value
    bool ok;
    int value = rule->contents().toInt(&ok);
    if (!ok) {
        value = 0;
    }

    auto numInput = valueStack->findChild<QSpinBox *>(QStringLiteral("QSpinBox"));

    if (numInput) {
        initNumInput(numInput, rule->field());
        numInput->blockSignals(true);
        numInput->setValue(value);
        numInput->blockSignals(false);
        valueStack->setCurrentWidget(numInput);
    }
    return true;
}

//---------------------------------------------------------------------------

bool NumericRuleWidgetHandler::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return false;
    }

    // raise the correct function widget
    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("numericRuleFuncCombo")));

    // raise the correct value widget
    auto numInput = valueStack->findChild<QSpinBox *>(QStringLiteral("QSpinBox"));

    if (numInput) {
        initNumInput(numInput, field);
        valueStack->setCurrentWidget(numInput);
    }
    return true;
}
