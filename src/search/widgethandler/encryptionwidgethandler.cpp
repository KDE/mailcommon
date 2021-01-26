/*
  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "encryptionwidgethandler.h"
#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>

using namespace MailCommon;

static const struct {
    SearchRule::Function id;
    const char *displayName;
} EncryptionFunctions[] = {{SearchRule::FuncEquals, I18N_NOOP("is")}, {SearchRule::FuncNotEqual, I18N_NOOP("is not")}};
static const int EncryptionFunctionCount = sizeof(EncryptionFunctions) / sizeof(*EncryptionFunctions);

EncryptionWidgetHandler::EncryptionWidgetHandler()
    : RuleWidgetHandler()
{
}

EncryptionWidgetHandler::~EncryptionWidgetHandler()
{
}

QWidget *EncryptionWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isBalooSearch) const
{
    Q_UNUSED(isBalooSearch)

    if (number != 0) {
        return nullptr;
    }

    auto combo = new QComboBox(functionStack);
    combo->setMinimumWidth(50);
    combo->setObjectName(QStringLiteral("encryptionRuleFuncCombo"));
    for (int i = 0; i < EncryptionFunctionCount; ++i) {
        combo->addItem(i18n(EncryptionFunctions[i].displayName));
    }
    combo->adjustSize();
    QObject::connect(combo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return combo;
}

QWidget *EncryptionWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
{
    Q_UNUSED(receiver)

    if (number != 0) {
        return nullptr;
    }

    auto lbl = new QLabel(i18n("encrypted"), valueStack);
    lbl->setObjectName(QStringLiteral("encryptionRuleValueLabel"));
    return lbl;
}

SearchRule::Function EncryptionWidgetHandler::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    if (!handlesField(field)) {
        return SearchRule::FuncNone;
    }

    const auto combo = functionStack->findChild<QComboBox *>(QStringLiteral("encryptionRuleFuncCombo"));
    if (combo && combo->currentIndex() >= 0) {
        return EncryptionFunctions[combo->currentIndex()].id;
    }

    return SearchRule::FuncNone;
}

QString EncryptionWidgetHandler::value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    Q_UNUSED(functionStack)
    Q_UNUSED(valueStack)
    if (!handlesField(field)) {
        return QString();
    }
    return QStringLiteral("is encrypted"); // dummy value
}

QString EncryptionWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    Q_UNUSED(functionStack)
    Q_UNUSED(valueStack)
    if (!handlesField(field)) {
        return QString();
    }

    return i18n("is encrypted");
}

bool EncryptionWidgetHandler::handlesField(const QByteArray &field) const
{
    return field == "<encryption>";
}

void EncryptionWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    const auto combo = functionStack->findChild<QComboBox *>(QStringLiteral("encryptionRuleFuncCombo"));
    if (combo) {
        const bool blocked = combo->blockSignals(true);
        combo->setCurrentIndex(0);
        combo->blockSignals(blocked);
    }

    Q_UNUSED(valueStack)
}

bool EncryptionWidgetHandler::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool isBalooSearch) const
{
    Q_UNUSED(isBalooSearch)
    if (!rule || !handlesField(rule->field())) {
        reset(functionStack, valueStack);
        return false;
    }
    update("<encryption>", functionStack, valueStack);

    const auto combo = functionStack->findChild<QComboBox *>(QStringLiteral("encryptionRuleFuncCombo"));
    if (combo) {
        const bool blocked = combo->blockSignals(true);
        for (int i = 0; i < EncryptionFunctionCount; ++i) {
            if (EncryptionFunctions[i].id == rule->function()) {
                combo->setCurrentIndex(i);
                break;
            }
        }
        combo->blockSignals(blocked);
        return true;
    }

    return true;
}

bool EncryptionWidgetHandler::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return false;
    }

    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("encryptionRuleFuncCombo")));
    valueStack->setCurrentWidget(valueStack->findChild<QWidget *>(QStringLiteral("encryptionRuleValueLabel")));

    return true;
}
