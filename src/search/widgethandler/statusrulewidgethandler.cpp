/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "statusrulewidgethandler.h"
#include "search/searchrule/searchrulestatus.h"

#include <QComboBox>
#include <QIcon>
#include <QStackedWidget>
#include <ki18n_version.h>
#if KI18N_VERSION >= QT_VERSION_CHECK(5, 89, 0)
#include <KLazyLocalizedString>
#undef I18N_NOOP
#define I18N_NOOP kli18n
#endif
using namespace MailCommon;

static const struct {
    SearchRule::Function id;
#if KI18N_VERSION < QT_VERSION_CHECK(5, 89, 0)
    const char *displayName;
#else
    const KLazyLocalizedString displayName;
#endif
} StatusFunctions[] = {{SearchRule::FuncContains, I18N_NOOP("is")}, {SearchRule::FuncContainsNot, I18N_NOOP("is not")}};
static const int StatusFunctionCount = sizeof(StatusFunctions) / sizeof(*StatusFunctions);

struct MessageStatusInfo {
#if KI18N_VERSION < QT_VERSION_CHECK(5, 89, 0)
    const char *context;
    const char *text;
#else
    const KLazyLocalizedString text;
#endif
    const char *icon;
};

// If you change the ordering here; also do it in the enum below
static const MessageStatusInfo StatusValues[] = {
    {I18NC_NOOP("message status", "Important"), "emblem-important"},
    {I18NC_NOOP("message status", "Action Item"), "mail-task"},
    {I18NC_NOOP("message status", "Unread"), "mail-unread"},
    {I18NC_NOOP("message status", "Read"), "mail-read"},
    {I18NC_NOOP("message status", "Deleted"), "mail-deleted"},
    {I18NC_NOOP("message status", "Replied"), "mail-replied"},
    {I18NC_NOOP("message status", "Forwarded"), "mail-forwarded"},
    {I18NC_NOOP("message status", "Queued"), "mail-queued"},
    {I18NC_NOOP("message status", "Sent"), "mail-sent"},
    {I18NC_NOOP("message status", "Watched"), "mail-thread-watch"},
    {I18NC_NOOP("message status", "Ignored"), "mail-thread-ignored"},
    {I18NC_NOOP("message status", "Spam"), "mail-mark-junk"},
    {I18NC_NOOP("message status", "Ham"), "mail-mark-notjunk"},
    {I18NC_NOOP("message status", "Has Attachment"), "mail-attachment"} // must be last
};

static const int StatusValueCount = sizeof(StatusValues) / sizeof(MessageStatusInfo);
// we want to show all status entries in the quick search bar, but only the
// ones up to attachment in the search/filter dialog, because there the
// attachment case is handled separately.
static const int StatusValueCountWithoutHidden = StatusValueCount - 1;

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
#if KI18N_VERSION < QT_VERSION_CHECK(5, 89, 0)
        funcCombo->addItem(i18n(StatusFunctions[i].displayName));
#else
        funcCombo->addItem(KLocalizedString(StatusFunctions[i].displayName).toString());
#endif
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
    for (int i = 0; i < StatusValueCountWithoutHidden; ++i) {
        if (StatusValues[i].icon != nullptr) {
#if KI18N_VERSION < QT_VERSION_CHECK(5, 89, 0)
            statusCombo->addItem(QIcon::fromTheme(QLatin1String(StatusValues[i].icon)), i18nc("message status", StatusValues[i].text));
#else
            statusCombo->addItem(QIcon::fromTheme(QLatin1String(StatusValues[i].icon)), KLocalizedString(StatusValues[i].text).toString());
#endif
        } else {
#if KI18N_VERSION < QT_VERSION_CHECK(5, 89, 0)
            statusCombo->addItem(i18nc("message status", StatusValues[i].text));
#else
            statusCombo->addItem(KLocalizedString(StatusValues[i].text).toString());
#endif
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
#if KI18N_VERSION < QT_VERSION_CHECK(5, 89, 0)
        return QString::fromLatin1(StatusValues[status].text);
#else
        return KLocalizedString(StatusValues[status].text).untranslatedText();
#endif
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
#if KI18N_VERSION < QT_VERSION_CHECK(5, 89, 0)
        return QString::fromLatin1(StatusValues[status].text);
#else
        return KLocalizedString(StatusValues[status].text).toString();
#endif
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
    for (; valueIndex < StatusValueCountWithoutHidden; ++valueIndex) {
#if KI18N_VERSION < QT_VERSION_CHECK(5, 89, 0)
        if (value == QString::fromLatin1(StatusValues[valueIndex].text)) {
            break;
        }
#else
        if (value == KLocalizedString(StatusValues[valueIndex].text).untranslatedText()) {
            break;
        }
#endif
    }

    const auto statusCombo = valueStack->findChild<QComboBox *>(QStringLiteral("statusRuleValueCombo"));

    if (statusCombo) {
        statusCombo->blockSignals(true);
        if (valueIndex < StatusValueCountWithoutHidden) {
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
