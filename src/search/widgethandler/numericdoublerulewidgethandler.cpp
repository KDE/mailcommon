/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "numericdoublerulewidgethandler.h"
#include "search/searchpattern.h"

#include <KLocalizedString>

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QStackedWidget>

using namespace MailCommon;

static const struct {
    SearchRule::Function id;
    const char *displayName;
} NumericDoubleFunctions[] = {{SearchRule::FuncEquals, I18N_NOOP("is equal to")},
                              {SearchRule::FuncNotEqual, I18N_NOOP("is not equal to")},
                              {SearchRule::FuncIsGreater, I18N_NOOP("is greater than")},
                              {SearchRule::FuncIsLessOrEqual, I18N_NOOP("is less than or equal to")},
                              {SearchRule::FuncIsLess, I18N_NOOP("is less than")},
                              {SearchRule::FuncIsGreaterOrEqual, I18N_NOOP("is greater than or equal to")}};
static const int NumericDoubleFunctionCount = sizeof(NumericDoubleFunctions) / sizeof(*NumericDoubleFunctions);

QWidget *NumericDoubleRuleWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool /*isBalooSearch*/) const
{
    if (number != 0) {
        return nullptr;
    }

    auto funcCombo = new QComboBox(functionStack);
    funcCombo->setMinimumWidth(50);
    funcCombo->setObjectName(QStringLiteral("numericDoubleRuleFuncCombo"));
    for (int i = 0; i < NumericDoubleFunctionCount; ++i) {
        funcCombo->addItem(i18n(NumericDoubleFunctions[i].displayName));
    }
    funcCombo->adjustSize();
    QObject::connect(funcCombo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return funcCombo;
}

//---------------------------------------------------------------------------

QWidget *NumericDoubleRuleWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
{
    if (number != 0) {
        return nullptr;
    }

    auto numInput = new QDoubleSpinBox(valueStack);
    numInput->setObjectName(QStringLiteral("QDoubleSpinBox"));
    QObject::connect(numInput, SIGNAL(valueChanged(double)), receiver, SLOT(slotValueChanged()));
    return numInput;
}

//---------------------------------------------------------------------------

SearchRule::Function NumericDoubleRuleWidgetHandler::currentFunction(const QStackedWidget *functionStack) const
{
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("numericDoubleRuleFuncCombo"));

    if (funcCombo && funcCombo->currentIndex() >= 0) {
        return NumericDoubleFunctions[funcCombo->currentIndex()].id;
    }

    return SearchRule::FuncNone;
}

//---------------------------------------------------------------------------

SearchRule::Function NumericDoubleRuleWidgetHandler::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    if (!handlesField(field)) {
        return SearchRule::FuncNone;
    }

    return currentFunction(functionStack);
}

//---------------------------------------------------------------------------

QString NumericDoubleRuleWidgetHandler::currentValue(const QStackedWidget *valueStack) const
{
    const QDoubleSpinBox *numInput = valueStack->findChild<QDoubleSpinBox *>(QStringLiteral("QDoubleSpinBox"));

    if (numInput) {
        return QString::number(int(numInput->value() * 1024));
    }

    return QString();
}

//---------------------------------------------------------------------------

QString NumericDoubleRuleWidgetHandler::value(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return QString();
    }

    return currentValue(valueStack);
}

//---------------------------------------------------------------------------

QString NumericDoubleRuleWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return QString();
    }

    return currentValue(valueStack);
}

//---------------------------------------------------------------------------

bool NumericDoubleRuleWidgetHandler::handlesField(const QByteArray &field) const
{
    return field == "<size>";
}

//---------------------------------------------------------------------------

void NumericDoubleRuleWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    // reset the function combo box
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("numericDoubleRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        funcCombo->setCurrentIndex(0);
        funcCombo->blockSignals(false);
    }

    // reset the value widget
    auto *numInput = valueStack->findChild<QDoubleSpinBox *>(QStringLiteral("QDoubleSpinBox"));

    if (numInput) {
        numInput->blockSignals(true);
        numInput->setValue(0.0);
        numInput->blockSignals(false);
    }
}

//---------------------------------------------------------------------------

void initDoubleNumInput(QDoubleSpinBox *numInput, const QByteArray &field)
{
    if (field == "<size>") {
        numInput->setMinimum(0);
        numInput->setSingleStep(1);
        numInput->setMaximum(10000000);
        numInput->setSuffix(i18nc("spinbox suffix: unit for kilobyte", " kB"));
    }
}

//---------------------------------------------------------------------------

bool NumericDoubleRuleWidgetHandler::setRule(QStackedWidget *functionStack,
                                             QStackedWidget *valueStack,
                                             const SearchRule::Ptr rule,
                                             bool /*isBalooSearch*/) const
{
    if (!rule || !handlesField(rule->field())) {
        reset(functionStack, valueStack);
        return false;
    }

    // set the function
    const SearchRule::Function func = rule->function();
    int funcIndex = 0;
    for (; funcIndex < NumericDoubleFunctionCount; ++funcIndex) {
        if (func == NumericDoubleFunctions[funcIndex].id) {
            break;
        }
    }

    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("numericDoubleRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        if (funcIndex < NumericDoubleFunctionCount) {
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

    auto *numInput = valueStack->findChild<QDoubleSpinBox *>(QStringLiteral("QDoubleSpinBox"));

    if (numInput) {
        initDoubleNumInput(numInput, rule->field());
        numInput->blockSignals(true);
        numInput->setValue(value / (1024.0));
        numInput->blockSignals(false);
        valueStack->setCurrentWidget(numInput);
    }
    return true;
}

//---------------------------------------------------------------------------

bool NumericDoubleRuleWidgetHandler::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return false;
    }

    // raise the correct function widget
    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("numericDoubleRuleFuncCombo")));

    // raise the correct value widget
    auto *numInput = valueStack->findChild<QDoubleSpinBox *>(QStringLiteral("QDoubleSpinBox"));

    if (numInput) {
        initDoubleNumInput(numInput, field);
        valueStack->setCurrentWidget(numInput);
    }
    return true;
}
