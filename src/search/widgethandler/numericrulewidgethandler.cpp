/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "numericrulewidgethandler.h"
#include "search/searchpattern.h"

#include <KLocalizedString>
#include <KPluralHandlingSpinBox>

#include <QComboBox>
#include <QStackedWidget>

using namespace MailCommon;

static const struct {
    SearchRule::Function id;
    const char *displayName;
} NumericFunctions[] = {{SearchRule::FuncEquals, I18N_NOOP("is equal to")},
                        {SearchRule::FuncNotEqual, I18N_NOOP("is not equal to")},
                        {SearchRule::FuncIsGreater, I18N_NOOP("is greater than")},
                        {SearchRule::FuncIsLessOrEqual, I18N_NOOP("is less than or equal to")},
                        {SearchRule::FuncIsLess, I18N_NOOP("is less than")},
                        {SearchRule::FuncIsGreaterOrEqual, I18N_NOOP("is greater than or equal to")}};
static const int NumericFunctionCount = sizeof(NumericFunctions) / sizeof(*NumericFunctions);

//---------------------------------------------------------------------------

QWidget *NumericRuleWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool /*isBalooSearch*/) const
{
    if (number != 0) {
        return nullptr;
    }

    auto funcCombo = new QComboBox(functionStack);
    funcCombo->setMinimumWidth(50);
    funcCombo->setObjectName(QStringLiteral("numericRuleFuncCombo"));
    for (int i = 0; i < NumericFunctionCount; ++i) {
        funcCombo->addItem(i18n(NumericFunctions[i].displayName));
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

    auto numInput = new KPluralHandlingSpinBox(valueStack);
    numInput->setObjectName(QStringLiteral("KPluralHandlingSpinBox"));
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
    const KPluralHandlingSpinBox *numInput = valueStack->findChild<KPluralHandlingSpinBox *>(QStringLiteral("KPluralHandlingSpinBox"));

    if (numInput) {
        return QString::number(numInput->value());
    }

    return QString();
}

//---------------------------------------------------------------------------

QString NumericRuleWidgetHandler::value(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return QString();
    }

    return currentValue(valueStack);
}

//---------------------------------------------------------------------------

QString NumericRuleWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return QString();
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
    auto *numInput = valueStack->findChild<KPluralHandlingSpinBox *>(QStringLiteral("KPluralHandlingSpinBox"));

    if (numInput) {
        numInput->blockSignals(true);
        numInput->setValue(0);
        numInput->blockSignals(false);
    }
}

//---------------------------------------------------------------------------

void initNumInput(KPluralHandlingSpinBox *numInput, const QByteArray &field)
{
    if (field == "<age in days>") {
        numInput->setMinimum(-10000);
        numInput->setSuffix(ki18ncp("Unit suffix where units are days.", " day", " days"));
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

    auto *numInput = valueStack->findChild<KPluralHandlingSpinBox *>(QStringLiteral("KPluralHandlingSpinBox"));

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
    auto *numInput = valueStack->findChild<KPluralHandlingSpinBox *>(QStringLiteral("KPluralHandlingSpinBox"));

    if (numInput) {
        initNumInput(numInput, field);
        valueStack->setCurrentWidget(numInput);
    }
    return true;
}
