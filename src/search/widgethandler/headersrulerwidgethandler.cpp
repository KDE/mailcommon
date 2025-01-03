/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "headersrulerwidgethandler.h"

#include "search/searchpattern.h"

#include <KLineEdit>
#include <KLocalizedString>

#include <KLazyLocalizedString>
#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
using namespace MailCommon;

// also see SearchRule::matches() and SearchRule::Function
// if you change the following strings!
static const struct {
    SearchRule::Function id;
    const KLazyLocalizedString displayName;
} HeaderFunctions[] = {{SearchRule::FuncContains, kli18n("contains")},
                       {SearchRule::FuncContainsNot, kli18n("does not contain")},
                       {SearchRule::FuncEquals, kli18n("equals")},
                       {SearchRule::FuncNotEqual, kli18n("does not equal")},
                       {SearchRule::FuncStartWith, kli18n("starts with")},
                       {SearchRule::FuncNotStartWith, kli18n("does not start with")},
                       {SearchRule::FuncEndWith, kli18n("ends with")},
                       {SearchRule::FuncNotEndWith, kli18n("does not end with")},

                       {SearchRule::FuncRegExp, kli18n("matches regular expr.")},
                       {SearchRule::FuncNotRegExp, kli18n("does not match reg. expr.")},
                       {SearchRule::FuncIsInAddressbook, kli18n("is in address book")},
                       {SearchRule::FuncIsNotInAddressbook, kli18n("is not in address book")}};
static const int HeadersFunctionCount = sizeof(HeaderFunctions) / sizeof(*HeaderFunctions);

//---------------------------------------------------------------------------

QWidget *HeadersRuleWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isBalooSearch) const
{
    if (number != 0) {
        return nullptr;
    }

    auto funcCombo = new QComboBox(functionStack);
    funcCombo->setMinimumWidth(50);
    funcCombo->setObjectName(QLatin1StringView("headerRuleFuncCombo"));
    for (int i = 0; i < HeadersFunctionCount; ++i) {
        if (!(isBalooSearch && (HeaderFunctions[i].id == SearchRule::FuncIsInAddressbook || HeaderFunctions[i].id == SearchRule::FuncIsNotInAddressbook))) {
            funcCombo->addItem(HeaderFunctions[i].displayName.toString());
        }
    }
    funcCombo->adjustSize();
    QObject::connect(funcCombo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return funcCombo;
}

//---------------------------------------------------------------------------

QWidget *HeadersRuleWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
{
    if (number == 0) {
        auto lineEdit = new KLineEdit(valueStack);
        lineEdit->setClearButtonEnabled(true);
        lineEdit->setTrapReturnKey(true);
        lineEdit->setObjectName(QLatin1StringView("regExpLineEdit"));
        QObject::connect(lineEdit, SIGNAL(textChanged(QString)), receiver, SLOT(slotValueChanged()));
        QObject::connect(lineEdit, SIGNAL(returnPressed()), receiver, SLOT(slotReturnPressed()));
        return lineEdit;
    }

    // blank QLabel to hide value widget for in-address-book rule
    if (number == 1) {
        auto label = new QLabel(valueStack);
        label->setObjectName(QLatin1StringView("headerRuleValueHider"));
        label->setBuddy(valueStack);
        return label;
    }
    return nullptr;
}

//---------------------------------------------------------------------------

SearchRule::Function HeadersRuleWidgetHandler::currentFunction(const QStackedWidget *functionStack) const
{
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("headerRuleFuncCombo"));

    if (funcCombo && funcCombo->currentIndex() >= 0) {
        return HeaderFunctions[funcCombo->currentIndex()].id;
    }

    return SearchRule::FuncNone;
}

//---------------------------------------------------------------------------

SearchRule::Function HeadersRuleWidgetHandler::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    if (!handlesField(field)) {
        return SearchRule::FuncNone;
    }
    return currentFunction(functionStack);
}

//---------------------------------------------------------------------------
QString HeadersRuleWidgetHandler::currentValue(const QStackedWidget *valueStack, SearchRule::Function func) const
{
    Q_UNUSED(func)

    const KLineEdit *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));

    if (lineEdit) {
        return lineEdit->text();
    }

    // or anything else, like addressbook
    return {};
}

//---------------------------------------------------------------------------

QString HeadersRuleWidgetHandler::value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return {};
    }
    SearchRule::Function func = currentFunction(functionStack);
    if (func == SearchRule::FuncIsInAddressbook) {
        return QStringLiteral("is in address book"); // just a non-empty dummy value
    } else if (func == SearchRule::FuncIsNotInAddressbook) {
        return QStringLiteral("is not in address book"); // just a non-empty dummy value
    } else {
        return currentValue(valueStack, func);
    }
}

//---------------------------------------------------------------------------

QString HeadersRuleWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return {};
    }

    SearchRule::Function func = currentFunction(functionStack);

    if (func == SearchRule::FuncIsInAddressbook) {
        return i18n("is in address book");
    } else if (func == SearchRule::FuncIsNotInAddressbook) {
        return i18n("is not in address book");
    } else {
        return currentValue(valueStack, func);
    }
}

//---------------------------------------------------------------------------

bool HeadersRuleWidgetHandler::handlesField(const QByteArray &field) const
{
    return field == "To" || field == "From" || field == "CC" || field == "<recipients>";
}

//---------------------------------------------------------------------------

void HeadersRuleWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    // reset the function combo box
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("headerRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        funcCombo->setCurrentIndex(0);
        funcCombo->blockSignals(false);
    }

    // reset the value widget
    auto lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));
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

bool HeadersRuleWidgetHandler::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool isBalooSearch) const
{
    if (!rule || !handlesField(rule->field())) {
        reset(functionStack, valueStack);
        return false;
    }

    const SearchRule::Function func = rule->function();
    if ((isBalooSearch && (func == SearchRule::FuncIsInAddressbook || func == SearchRule::FuncIsNotInAddressbook))) {
        reset(functionStack, valueStack);
        return false;
    }

    int i = 0;
    for (; i < HeadersFunctionCount; ++i) {
        if (func == HeaderFunctions[i].id) {
            break;
        }
    }

    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("headerRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        if (i < HeadersFunctionCount) {
            funcCombo->setCurrentIndex(i);
        } else {
            funcCombo->setCurrentIndex(0);
        }
        funcCombo->blockSignals(false);
        functionStack->setCurrentWidget(funcCombo);
    }

    if (func == SearchRule::FuncIsInAddressbook || func == SearchRule::FuncIsNotInAddressbook) {
        auto w = valueStack->findChild<QWidget *>(QStringLiteral("headerRuleValueHider"));
        valueStack->setCurrentWidget(w);
    } else {
        auto lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));

        if (lineEdit) {
            lineEdit->blockSignals(true);
            lineEdit->setText(rule->contents());
            lineEdit->blockSignals(false);
            lineEdit->setClearButtonEnabled(false);
            lineEdit->setClearButtonEnabled(true);
            valueStack->setCurrentWidget(lineEdit);
        }
    }
    return true;
}

//---------------------------------------------------------------------------

bool HeadersRuleWidgetHandler::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return false;
    }

    // raise the correct function widget
    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("headerRuleFuncCombo")));

    // raise the correct value widget
    SearchRule::Function func = currentFunction(functionStack);
    if (func == SearchRule::FuncIsInAddressbook || func == SearchRule::FuncIsNotInAddressbook) {
        valueStack->setCurrentWidget(valueStack->findChild<QWidget *>(QStringLiteral("headerRuleValueHider")));
    } else {
        auto lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));

        if (lineEdit) {
            valueStack->setCurrentWidget(lineEdit);
        }
    }
    return true;
}
