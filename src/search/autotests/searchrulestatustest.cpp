/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "searchrulestatustest.h"
#include "../searchrule/searchrulestatus.h"
#include <QTest>

SearchRuleStatusTest::SearchRuleStatusTest(QObject *parent)
    : QObject(parent)
{
}

void SearchRuleStatusTest::shouldHaveDefaultValue()
{
    MailCommon::SearchRuleStatus ruleStatus;
    QVERIFY(ruleStatus.contents().isEmpty());
    QVERIFY(ruleStatus.field().isEmpty());
    QCOMPARE(ruleStatus.function(), MailCommon::SearchRule::FuncContains);
    QVERIFY(ruleStatus.isEmpty());
}

void SearchRuleStatusTest::shouldHaveRequirePart()
{
    MailCommon::SearchRuleStatus ruleStatus;
    QCOMPARE(ruleStatus.requiredPart(), MailCommon::SearchRule::Envelope);
}

void SearchRuleStatusTest::shouldMatchStatus()
{
}

void SearchRuleStatusTest::shouldMatchStatus_data()
{
}

void SearchRuleStatusTest::shouldBeEmpty()
{
    MailCommon::SearchRuleStatus searchrule;
    QVERIFY(searchrule.isEmpty());
    searchrule = MailCommon::SearchRuleStatus(QByteArray(), MailCommon::SearchRule::FuncContains, QStringLiteral("foo"));
    QVERIFY(searchrule.isEmpty());
    searchrule = MailCommon::SearchRuleStatus(QByteArray("<tag>"), MailCommon::SearchRule::FuncContains, QString());
    QVERIFY(searchrule.isEmpty());

    searchrule = MailCommon::SearchRuleStatus(QByteArray("<tag>"), MailCommon::SearchRule::FuncContains, QStringLiteral("foo"));
    QVERIFY(!searchrule.isEmpty());
}

QTEST_MAIN(SearchRuleStatusTest)
