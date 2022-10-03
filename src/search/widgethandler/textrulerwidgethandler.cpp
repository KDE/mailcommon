/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "textrulerwidgethandler.h"

#include <KLineEdit>
#include <KLocalizedString>
#include <QComboBox>
#include <QStackedWidget>

using namespace MailCommon;

#include <KLazyLocalizedString>
#include <QLabel>

// also see SearchRule::matches() and SearchRule::Function
// if you change the following strings!
static const struct {
    SearchRule::Function id;
    const KLazyLocalizedString displayName;
} TextFunctions[] = {{SearchRule::FuncContains, kli18n("contains")},
                     {SearchRule::FuncContainsNot, kli18n("does not contain")},
                     {SearchRule::FuncEquals, kli18n("equals")},
                     {SearchRule::FuncNotEqual, kli18n("does not equal")},
                     {SearchRule::FuncStartWith, kli18n("starts with")},
                     {SearchRule::FuncNotStartWith, kli18n("does not start with")},
                     {SearchRule::FuncEndWith, kli18n("ends with")},
                     {SearchRule::FuncNotEndWith, kli18n("does not end with")},

                     {SearchRule::FuncRegExp, kli18n("matches regular expr.")},
                     {SearchRule::FuncNotRegExp, kli18n("does not match reg. expr.")}};
static const int TextFunctionCount = sizeof(TextFunctions) / sizeof(*TextFunctions);

//---------------------------------------------------------------------------

QWidget *TextRuleWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool /*isBalooSearch*/) const
{
    if (number != 0) {
        return nullptr;
    }

    auto funcCombo = new QComboBox(functionStack);
    funcCombo->setMinimumWidth(50);
    funcCombo->setObjectName(QStringLiteral("textRuleFuncCombo"));
    for (int i = 0; i < TextFunctionCount; ++i) {
        funcCombo->addItem(TextFunctions[i].displayName.toString());
    }
    funcCombo->adjustSize();
    QObject::connect(funcCombo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return funcCombo;
}

//---------------------------------------------------------------------------

QWidget *TextRuleWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
{
    if (number == 0) {
        auto lineEdit = new KLineEdit(valueStack);
        lineEdit->setClearButtonEnabled(true);
        lineEdit->setTrapReturnKey(true);
        lineEdit->setObjectName(QStringLiteral("regExpLineEdit"));
        QObject::connect(lineEdit, SIGNAL(textChanged(QString)), receiver, SLOT(slotValueChanged()));
        QObject::connect(lineEdit, SIGNAL(returnPressed()), receiver, SLOT(slotReturnPressed()));
        return lineEdit;
    }

    // blank QLabel to hide value widget for in-address-book rule
    if (number == 1) {
        auto label = new QLabel(valueStack);
        label->setObjectName(QStringLiteral("textRuleValueHider"));
        label->setBuddy(valueStack);
        return label;
    }
    return nullptr;
}

//---------------------------------------------------------------------------

SearchRule::Function TextRuleWidgetHandler::currentFunction(const QStackedWidget *functionStack) const
{
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("textRuleFuncCombo"));

    if (funcCombo && funcCombo->currentIndex() >= 0) {
        return TextFunctions[funcCombo->currentIndex()].id;
    }

    return SearchRule::FuncNone;
}

//---------------------------------------------------------------------------

SearchRule::Function TextRuleWidgetHandler::function(const QByteArray &, const QStackedWidget *functionStack) const
{
    return currentFunction(functionStack);
}

//---------------------------------------------------------------------------

QString TextRuleWidgetHandler::currentValue(const QStackedWidget *valueStack, SearchRule::Function) const
{
    // in other cases of func it is a lineedit
    const KLineEdit *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));

    if (lineEdit) {
        return lineEdit->text();
    }

    // or anything else, like addressbook
    return {};
}

//---------------------------------------------------------------------------

QString TextRuleWidgetHandler::value(const QByteArray &, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    SearchRule::Function func = currentFunction(functionStack);
    return currentValue(valueStack, func);
}

//---------------------------------------------------------------------------

QString TextRuleWidgetHandler::prettyValue(const QByteArray &, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    SearchRule::Function func = currentFunction(functionStack);
    return currentValue(valueStack, func);
}

//---------------------------------------------------------------------------

bool TextRuleWidgetHandler::handlesField(const QByteArray &) const
{
    return true; // we handle all fields (as fallback)
}

//---------------------------------------------------------------------------

void TextRuleWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    // reset the function combo box
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("textRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        funcCombo->setCurrentIndex(0);
        funcCombo->blockSignals(false);
    }

    // reset the value widget
    auto *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));
    if (lineEdit) {
        lineEdit->blockSignals(true);
        lineEdit->clear();
        lineEdit->blockSignals(false);
        lineEdit->setClearButtonEnabled(false);
        lineEdit->setClearButtonEnabled(true);
        valueStack->setCurrentWidget(lineEdit);
    }
}

//---------------------------------------------------------------------------

bool TextRuleWidgetHandler::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool /*isBalooSearch*/) const
{
    if (!rule) {
        reset(functionStack, valueStack);
        return false;
    }

    const SearchRule::Function func = rule->function();
    int i = 0;
    for (; i < TextFunctionCount; ++i) {
        if (func == TextFunctions[i].id) {
            break;
        }
    }

    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("textRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        if (i < TextFunctionCount) {
            funcCombo->setCurrentIndex(i);
        } else {
            funcCombo->setCurrentIndex(0);
        }
        funcCombo->blockSignals(false);
        functionStack->setCurrentWidget(funcCombo);
    }
    auto *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));

    if (lineEdit) {
        lineEdit->blockSignals(true);
        lineEdit->setText(rule->contents());
        lineEdit->blockSignals(false);
        lineEdit->setClearButtonEnabled(false);
        lineEdit->setClearButtonEnabled(true);
        valueStack->setCurrentWidget(lineEdit);
    }
    return true;
}

//---------------------------------------------------------------------------

bool TextRuleWidgetHandler::update(const QByteArray &, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    // raise the correct function widget
    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("textRuleFuncCombo")));

    // raise the correct value widget
    SearchRule::Function func = currentFunction(functionStack);
    if (func == SearchRule::FuncIsInAddressbook || func == SearchRule::FuncIsNotInAddressbook) {
        valueStack->setCurrentWidget(valueStack->findChild<QWidget *>(QStringLiteral("textRuleValueHider")));
    } else {
        auto *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));

        if (lineEdit) {
            valueStack->setCurrentWidget(lineEdit);
        }
    }
    return true;
}
