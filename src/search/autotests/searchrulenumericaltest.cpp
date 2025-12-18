/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "searchrulenumericaltest.h"
#include "../searchrule/searchrulenumerical.h"
#include <QTest>
SearchRuleNumericalTest::SearchRuleNumericalTest(QObject *parent)
    : QObject(parent)
{
}

void SearchRuleNumericalTest::shouldHaveDefaultValue()
{
    MailCommon::SearchRuleNumerical ruleStatus;
    QVERIFY(ruleStatus.contents().isEmpty());
    QVERIFY(ruleStatus.field().isEmpty());
    QCOMPARE(ruleStatus.function(), MailCommon::SearchRule::FuncContains);
    QVERIFY(ruleStatus.isEmpty());
}

void SearchRuleNumericalTest::shouldBeEmpty()
{
    MailCommon::SearchRuleNumerical ruleStatus(QByteArray(), MailCommon::SearchRule::FuncContains, QStringLiteral("foo"));
    QVERIFY(ruleStatus.isEmpty());
    ruleStatus = MailCommon::SearchRuleNumerical(QByteArray(), MailCommon::SearchRule::FuncContains, QStringLiteral("0"));
    QVERIFY(!ruleStatus.isEmpty());
}

void SearchRuleNumericalTest::shouldHaveRequirePart()
{
    MailCommon::SearchRuleNumerical ruleStatus;
    QCOMPARE(ruleStatus.requiredPart(), MailCommon::SearchRule::Envelope);
}

void SearchRuleNumericalTest::shouldMatchNumericalsize_data()
{
#if 0
    QTest::addColumn<MailCommon::SearchRule::Function>("function");
    QTest::addColumn<long>("value");
    QTest::addColumn<long>("matchvalue");
    QTest::addColumn<bool>("match");
#endif
}

void SearchRuleNumericalTest::shouldMatchNumericalsize()
{
#if 0
    QFETCH(MailCommon::SearchRule::Function, function);
    QFETCH(long, value);
    QFETCH(long, matchvalue);
    QFETCH(bool, match);
    MailCommon::SearchRuleNumerical searchrule("<size>", function, QString::number(value));
    auto msgPtr = std::make_shared<KMime::Message>();
    msgPtr->date(KMime::CreatePolicy::Create)->setDateTime(maildate);
    Akonadi::Item item;
    item.setPayload<std::shared_ptr<KMime::Message>>(msgPtr);
    QCOMPARE(searchrule.matches(item), match);
#endif
}

QTEST_MAIN(SearchRuleNumericalTest)

#include "moc_searchrulenumericaltest.cpp"
