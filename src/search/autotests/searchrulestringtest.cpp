/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "searchrulestringtest.h"
#include "../searchrule/searchrulestring.h"
#include <QTest>

SearchRuleStringTest::SearchRuleStringTest(QObject *parent)
    : QObject(parent)
{
}

void SearchRuleStringTest::shouldHaveDefaultValue()
{
    MailCommon::SearchRuleString searchrule;
    QCOMPARE(searchrule.function(), MailCommon::SearchRule::FuncContains);
    QVERIFY(searchrule.isEmpty());
}

void SearchRuleStringTest::shouldHaveRequirePart()
{
    QFETCH(QByteArray, field);
    QFETCH(MailCommon::SearchRule::RequiredPart, requiredpart);
    MailCommon::SearchRuleString ruleStatus(field);
    QCOMPARE(ruleStatus.requiredPart(), requiredpart);
}

void SearchRuleStringTest::shouldHaveRequirePart_data()
{
    QTest::addColumn<QByteArray>("field");
    QTest::addColumn<MailCommon::SearchRule::RequiredPart>("requiredpart");

    QTest::newRow("recipient") << QByteArray("<recipients>") << MailCommon::SearchRule::Envelope;
    QTest::newRow("status") << QByteArray("<status>") << MailCommon::SearchRule::Envelope;
    QTest::newRow("tag") << QByteArray("<tag>") << MailCommon::SearchRule::Envelope;
    QTest::newRow("subject") << QByteArray("subject") << MailCommon::SearchRule::Envelope;
    QTest::newRow("from") << QByteArray("from") << MailCommon::SearchRule::Envelope;
    QTest::newRow("sender") << QByteArray("sender") << MailCommon::SearchRule::Envelope;
    QTest::newRow("reply-to") << QByteArray("reply-to") << MailCommon::SearchRule::Envelope;
    QTest::newRow("to") << QByteArray("to") << MailCommon::SearchRule::Envelope;
    QTest::newRow("cc") << QByteArray("cc") << MailCommon::SearchRule::Envelope;
    QTest::newRow("in-reply-to") << QByteArray("in-reply-to") << MailCommon::SearchRule::Envelope;
    QTest::newRow("message-id") << QByteArray("message-id") << MailCommon::SearchRule::Envelope;
    QTest::newRow("references") << QByteArray("references") << MailCommon::SearchRule::Envelope;

    QTest::newRow("messages") << QByteArray("<message>") << MailCommon::SearchRule::CompleteMessage;
    QTest::newRow("body") << QByteArray("<body>") << MailCommon::SearchRule::CompleteMessage;

    QTest::newRow("unknown") << QByteArray("unknown") << MailCommon::SearchRule::Header;
}

void SearchRuleStringTest::shouldMatchString()
{
}

void SearchRuleStringTest::shouldMatchString_data()
{
}

void SearchRuleStringTest::shouldBeEmpty()
{
    MailCommon::SearchRuleString searchrule;
    QVERIFY(searchrule.isEmpty());
    searchrule = MailCommon::SearchRuleString(QByteArray(), MailCommon::SearchRule::FuncContains, QStringLiteral("foo"));
    QVERIFY(searchrule.isEmpty());
    searchrule = MailCommon::SearchRuleString(QByteArray("<tag>"), MailCommon::SearchRule::FuncContains, QString());
    QVERIFY(searchrule.isEmpty());

    searchrule = MailCommon::SearchRuleString(QByteArray("<tag>"), MailCommon::SearchRule::FuncContains, QStringLiteral("foo"));
    QVERIFY(!searchrule.isEmpty());
}

QTEST_MAIN(SearchRuleStringTest)

#include "moc_searchrulestringtest.cpp"
