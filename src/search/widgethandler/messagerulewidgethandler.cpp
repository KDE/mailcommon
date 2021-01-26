/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "messagerulewidgethandler.h"
#include "search/searchpattern.h"

#include <KLineEdit>
#include <KLocalizedString>
#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>

using namespace MailCommon;

// also see SearchRule::matches() and SearchRule::Function
// if you change the following strings!
static const struct {
    SearchRule::Function id;
    const char *displayName;
} MessageFunctions[] = {
    {SearchRule::FuncContains, I18N_NOOP("contains")},
    {SearchRule::FuncContainsNot, I18N_NOOP("does not contain")},
    {SearchRule::FuncRegExp, I18N_NOOP("matches regular expr.")},
    {SearchRule::FuncNotRegExp, I18N_NOOP("does not match reg. expr.")},
    {SearchRule::FuncHasAttachment, I18N_NOOP("has an attachment")},
    {SearchRule::FuncHasNoAttachment, I18N_NOOP("has no attachment")},
};
static const int MessageFunctionCount = sizeof(MessageFunctions) / sizeof(*MessageFunctions);

//---------------------------------------------------------------------------

QWidget *MessageRuleWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isBalooSearch) const
{
    if (number != 0) {
        return nullptr;
    }

    auto funcCombo = new QComboBox(functionStack);
    funcCombo->setMinimumWidth(50);
    funcCombo->setObjectName(QStringLiteral("messageRuleFuncCombo"));
    for (int i = 0; i < MessageFunctionCount; ++i) {
        if (!(isBalooSearch && (MessageFunctions[i].id == SearchRule::FuncHasAttachment || MessageFunctions[i].id == SearchRule::FuncHasNoAttachment))) {
            funcCombo->addItem(i18n(MessageFunctions[i].displayName));
        }
    }
    funcCombo->adjustSize();
    QObject::connect(funcCombo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return funcCombo;
}

//---------------------------------------------------------------------------

QWidget *MessageRuleWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
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

    // blank QLabel to hide value widget for has-attachment rule
    if (number == 1) {
        auto label = new QLabel(valueStack);
        label->setObjectName(QStringLiteral("textRuleValueHider"));
        label->setBuddy(valueStack);
        return label;
    }

    return nullptr;
}

//---------------------------------------------------------------------------

SearchRule::Function MessageRuleWidgetHandler::currentFunction(const QStackedWidget *functionStack) const
{
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("messageRuleFuncCombo"));

    if (funcCombo && funcCombo->currentIndex() >= 0) {
        return MessageFunctions[funcCombo->currentIndex()].id;
    }

    return SearchRule::FuncNone;
}

//---------------------------------------------------------------------------

SearchRule::Function MessageRuleWidgetHandler::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    if (!handlesField(field)) {
        return SearchRule::FuncNone;
    }

    return currentFunction(functionStack);
}

//---------------------------------------------------------------------------

QString MessageRuleWidgetHandler::currentValue(const QStackedWidget *valueStack, SearchRule::Function) const
{
    const KLineEdit *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));

    if (lineEdit) {
        return lineEdit->text();
    }

    return QString();
}

//---------------------------------------------------------------------------

QString MessageRuleWidgetHandler::value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return QString();
    }

    SearchRule::Function func = currentFunction(functionStack);
    if (func == SearchRule::FuncHasAttachment) {
        return QStringLiteral("has an attachment"); // just a non-empty dummy value
    } else if (func == SearchRule::FuncHasNoAttachment) {
        return QStringLiteral("has no attachment"); // just a non-empty dummy value
    } else {
        return currentValue(valueStack, func);
    }
}

//---------------------------------------------------------------------------

QString MessageRuleWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return QString();
    }

    SearchRule::Function func = currentFunction(functionStack);
    if (func == SearchRule::FuncHasAttachment) {
        return i18n("has an attachment");
    } else if (func == SearchRule::FuncHasNoAttachment) {
        return i18n("has no attachment");
    } else {
        return currentValue(valueStack, func);
    }
}

//---------------------------------------------------------------------------

bool MessageRuleWidgetHandler::handlesField(const QByteArray &field) const
{
    return field == "<message>";
}

//---------------------------------------------------------------------------

void MessageRuleWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    // reset the function combo box
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("messageRuleFuncCombo"));

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

bool MessageRuleWidgetHandler::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool isBalooSearch) const
{
    if (!rule || !handlesField(rule->field())) {
        reset(functionStack, valueStack);
        return false;
    }

    const SearchRule::Function func = rule->function();

    if ((isBalooSearch && (func == SearchRule::FuncHasAttachment || func == SearchRule::FuncHasNoAttachment))) {
        reset(functionStack, valueStack);
        return false;
    }

    int i = 0;
    for (; i < MessageFunctionCount; ++i) {
        if (func == MessageFunctions[i].id) {
            break;
        }
    }

    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("messageRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        if (i < MessageFunctionCount) {
            funcCombo->setCurrentIndex(i);
        } else {
            funcCombo->setCurrentIndex(0);
        }
        funcCombo->blockSignals(false);
        functionStack->setCurrentWidget(funcCombo);
    }

    if (func == SearchRule::FuncHasAttachment || func == SearchRule::FuncHasNoAttachment) {
        auto *w = valueStack->findChild<QWidget *>(QStringLiteral("textRuleValueHider"));
        valueStack->setCurrentWidget(w);
    } else {
        auto *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));

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

bool MessageRuleWidgetHandler::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return false;
    }

    // raise the correct function widget
    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("messageRuleFuncCombo")));

    // raise the correct value widget
    SearchRule::Function func = currentFunction(functionStack);
    if (func == SearchRule::FuncHasAttachment || func == SearchRule::FuncHasNoAttachment) {
        auto *w = valueStack->findChild<QWidget *>(QStringLiteral("textRuleValueHider"));
        valueStack->setCurrentWidget(w);
    } else {
        auto *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("regExpLineEdit"));

        if (lineEdit) {
            valueStack->setCurrentWidget(lineEdit);
        }
    }
    return true;
}
