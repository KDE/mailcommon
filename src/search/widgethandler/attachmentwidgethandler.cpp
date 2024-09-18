/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "attachmentwidgethandler.h"
#include <KLazyLocalizedString>
#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
using namespace MailCommon;

static const struct {
    SearchRule::Function id;
    const KLazyLocalizedString displayName;
} AttachmentFunctions[] = {{SearchRule::FuncEquals, kli18n("has")}, {SearchRule::FuncNotEqual, kli18n("has not")}};
static const int EncryptionFunctionCount = sizeof(AttachmentFunctions) / sizeof(*AttachmentFunctions);

AttachmentWidgetHandler::AttachmentWidgetHandler()
    : RuleWidgetHandler()
{
}

AttachmentWidgetHandler::~AttachmentWidgetHandler() = default;

QWidget *AttachmentWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isBalooSearch) const
{
    Q_UNUSED(isBalooSearch)

    if (number != 0) {
        return nullptr;
    }

    auto combo = new QComboBox(functionStack);
    combo->setMinimumWidth(50);
    combo->setObjectName(QLatin1StringView("attachmentRuleFuncCombo"));
    for (int i = 0; i < EncryptionFunctionCount; ++i) {
        combo->addItem(AttachmentFunctions[i].displayName.toString());
    }
    combo->adjustSize();
    QObject::connect(combo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return combo;
}

QWidget *AttachmentWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
{
    Q_UNUSED(receiver)

    if (number != 0) {
        return nullptr;
    }

    auto lbl = new QLabel(i18nc("@label:textbox", "attachments"), valueStack);
    lbl->setObjectName(QLatin1StringView("attachmentRuleValueLabel"));
    return lbl;
}

SearchRule::Function AttachmentWidgetHandler::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    if (!handlesField(field)) {
        return SearchRule::FuncNone;
    }

    const auto combo = functionStack->findChild<QComboBox *>(QStringLiteral("attachmentRuleFuncCombo"));
    if (combo && combo->currentIndex() >= 0) {
        return AttachmentFunctions[combo->currentIndex()].id;
    }

    return SearchRule::FuncNone;
}

QString AttachmentWidgetHandler::value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    Q_UNUSED(functionStack)
    Q_UNUSED(valueStack)
    if (!handlesField(field)) {
        return {};
    }
    return QStringLiteral("has attachment"); // dummy value
}

QString AttachmentWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    Q_UNUSED(functionStack)
    Q_UNUSED(valueStack)
    if (!handlesField(field)) {
        return {};
    }

    return i18n("has attachment");
}

bool AttachmentWidgetHandler::handlesField(const QByteArray &field) const
{
    return field == "<attachment>";
}

void AttachmentWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    const auto combo = functionStack->findChild<QComboBox *>(QStringLiteral("attachmentRuleFuncCombo"));
    if (combo) {
        const bool blocked = combo->blockSignals(true);
        combo->setCurrentIndex(0);
        combo->blockSignals(blocked);
    }

    Q_UNUSED(valueStack)
}

bool AttachmentWidgetHandler::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool isBalooSearch) const
{
    Q_UNUSED(isBalooSearch)
    if (!rule || !handlesField(rule->field())) {
        reset(functionStack, valueStack);
        return false;
    }
    update("<attachment>", functionStack, valueStack);

    const auto combo = functionStack->findChild<QComboBox *>(QStringLiteral("attachmentRuleFuncCombo"));
    if (combo) {
        const bool blocked = combo->blockSignals(true);
        for (int i = 0; i < EncryptionFunctionCount; ++i) {
            if (AttachmentFunctions[i].id == rule->function()) {
                combo->setCurrentIndex(i);
                break;
            }
        }
        combo->blockSignals(blocked);
        return true;
    }

    return true;
}

bool AttachmentWidgetHandler::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return false;
    }

    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("attachmentRuleFuncCombo")));
    valueStack->setCurrentWidget(valueStack->findChild<QWidget *>(QStringLiteral("attachmentRuleValueLabel")));

    return true;
}
