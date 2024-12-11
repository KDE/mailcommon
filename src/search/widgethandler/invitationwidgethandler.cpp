/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "invitationwidgethandler.h"
#include <KLazyLocalizedString>
#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
using namespace MailCommon;

static const struct {
    SearchRule::Function id;
    const KLazyLocalizedString displayName;
} InvitationFunctions[] = {{SearchRule::FuncEquals, kli18n("has")}, {SearchRule::FuncNotEqual, kli18n("has not")}};
static const int InvitationFunctionCount = sizeof(InvitationFunctions) / sizeof(*InvitationFunctions);

InvitationWidgetHandler::InvitationWidgetHandler()
    : RuleWidgetHandler()
{
}

InvitationWidgetHandler::~InvitationWidgetHandler() = default;

QWidget *InvitationWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isBalooSearch) const
{
    Q_UNUSED(isBalooSearch)

    if (number != 0) {
        return nullptr;
    }

    auto combo = new QComboBox(functionStack);
    combo->setMinimumWidth(50);
    combo->setObjectName(QLatin1StringView("invitationRuleFuncCombo"));
    for (int i = 0; i < InvitationFunctionCount; ++i) {
        combo->addItem(InvitationFunctions[i].displayName.toString());
    }
    combo->adjustSize();
    QObject::connect(combo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return combo;
}

QWidget *InvitationWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
{
    Q_UNUSED(receiver)

    if (number != 0) {
        return nullptr;
    }

    auto lbl = new QLabel(i18nc("@label:textbox", "invitation"), valueStack);
    lbl->setObjectName(QLatin1StringView("invitationRuleValueLabel"));
    return lbl;
}

SearchRule::Function InvitationWidgetHandler::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    if (!handlesField(field)) {
        return SearchRule::FuncNone;
    }

    const auto combo = functionStack->findChild<QComboBox *>(QStringLiteral("invitationRuleFuncCombo"));
    if (combo && combo->currentIndex() >= 0) {
        return InvitationFunctions[combo->currentIndex()].id;
    }

    return SearchRule::FuncNone;
}

QString InvitationWidgetHandler::value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    Q_UNUSED(functionStack)
    Q_UNUSED(valueStack)
    if (!handlesField(field)) {
        return {};
    }
    return QStringLiteral("has invitation"); // dummy value
}

QString InvitationWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    Q_UNUSED(functionStack)
    Q_UNUSED(valueStack)
    if (!handlesField(field)) {
        return {};
    }

    return i18n("has invitation");
}

bool InvitationWidgetHandler::handlesField(const QByteArray &field) const
{
    return field == "<invitation>";
}

void InvitationWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    const auto combo = functionStack->findChild<QComboBox *>(QStringLiteral("invitationRuleFuncCombo"));
    if (combo) {
        const bool blocked = combo->blockSignals(true);
        combo->setCurrentIndex(0);
        combo->blockSignals(blocked);
    }

    Q_UNUSED(valueStack)
}

bool InvitationWidgetHandler::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool isBalooSearch) const
{
    Q_UNUSED(isBalooSearch)
    if (!rule || !handlesField(rule->field())) {
        reset(functionStack, valueStack);
        return false;
    }
    update("<invitation>", functionStack, valueStack);

    const auto combo = functionStack->findChild<QComboBox *>(QStringLiteral("invitationRuleFuncCombo"));
    if (combo) {
        const bool blocked = combo->blockSignals(true);
        for (int i = 0; i < InvitationFunctionCount; ++i) {
            if (InvitationFunctions[i].id == rule->function()) {
                combo->setCurrentIndex(i);
                break;
            }
        }
        combo->blockSignals(blocked);
        return true;
    }

    return true;
}

bool InvitationWidgetHandler::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return false;
    }

    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("invitationRuleFuncCombo")));
    valueStack->setCurrentWidget(valueStack->findChild<QWidget *>(QStringLiteral("invitationRuleValueLabel")));

    return true;
}
