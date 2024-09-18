/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "searchruleattachmenttest.h"
#include "searchrule/searchruleattachment.h"
#include <QTest>
QTEST_MAIN(SearchRuleAttachmentTest)

SearchRuleAttachmentTest::SearchRuleAttachmentTest(QObject *parent)
    : QObject(parent)
{
}

void SearchRuleAttachmentTest::shouldHaveDefaultValue()
{
    MailCommon::SearchRuleAttachment ruleAttachment;
    QVERIFY(ruleAttachment.contents().isEmpty());
    QVERIFY(ruleAttachment.field().isEmpty());
    QCOMPARE(ruleAttachment.function(), MailCommon::SearchRule::FuncEquals);
    QVERIFY(!ruleAttachment.isEmpty());
}

void SearchRuleAttachmentTest::shouldHaveRequirePart()
{
    MailCommon::SearchRuleAttachment ruleStatus;
    QCOMPARE(ruleStatus.requiredPart(), MailCommon::SearchRule::Envelope);
}

void SearchRuleAttachmentTest::shouldBeEmpty()
{
    MailCommon::SearchRuleAttachment searchrule;
    QVERIFY(!searchrule.isEmpty());
    searchrule = MailCommon::SearchRuleAttachment(QByteArray(), MailCommon::SearchRule::FuncEquals, QStringLiteral("foo"));
    QVERIFY(!searchrule.isEmpty());
    searchrule = MailCommon::SearchRuleAttachment(QByteArray("<attachment>"), MailCommon::SearchRule::FuncEquals, QString());
    QVERIFY(!searchrule.isEmpty());

    searchrule = MailCommon::SearchRuleAttachment(QByteArray("<attachment>"), MailCommon::SearchRule::FuncEquals, QStringLiteral("foo"));
    QVERIFY(!searchrule.isEmpty());
}

#include "moc_searchruleattachmenttest.cpp"
