/*
  SPDX-FileCopyrightText: 2015 Tomas Trnka <tomastrnka@gmx.com>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "searchpatterntest.h"
#include "../searchpattern.h"
#include <QTest>

SearchPatternTest::SearchPatternTest(QObject *parent)
    : QObject(parent)
{
}

SearchPatternTest::~SearchPatternTest()
{
}

void SearchPatternTest::shouldRuleRequirePart_data()
{
    QTest::addColumn<QByteArray>("field");
    QTest::addColumn<MailCommon::SearchRule::RequiredPart>("part");

    // SearchRuleStatus
    QTest::newRow("status") << QByteArray("<status>") << MailCommon::SearchRule::Envelope;

    // SearchRuleNumerical
    QTest::newRow("numerical") << QByteArray("<size>") << MailCommon::SearchRule::Envelope;

    // SearchRuleDate
    QTest::newRow("date") << QByteArray("<date>") << MailCommon::SearchRule::Envelope;

    // SearchRuleString
    QTest::newRow("recipients") << QByteArray("<recipients>") << MailCommon::SearchRule::Envelope;
    QTest::newRow("tag") << QByteArray("<tag>") << MailCommon::SearchRule::Envelope;
    QTest::newRow("subject") << QByteArray("subject") << MailCommon::SearchRule::Envelope;
    QTest::newRow("from") << QByteArray("from") << MailCommon::SearchRule::Envelope;
    QTest::newRow("sender") << QByteArray("sender") << MailCommon::SearchRule::Envelope;
    QTest::newRow("reply-to") << QByteArray("reply-to") << MailCommon::SearchRule::Envelope;
    QTest::newRow("to") << QByteArray("to") << MailCommon::SearchRule::Envelope;
    QTest::newRow("cc") << QByteArray("cc") << MailCommon::SearchRule::Envelope;
    QTest::newRow("bcc") << QByteArray("bcc") << MailCommon::SearchRule::Envelope;
    QTest::newRow("in-reply-to") << QByteArray("in-reply-to") << MailCommon::SearchRule::Envelope;
    QTest::newRow("message-id") << QByteArray("message-id") << MailCommon::SearchRule::Envelope;
    QTest::newRow("references") << QByteArray("references") << MailCommon::SearchRule::Envelope;

    QTest::newRow("any header") << QByteArray("<any header>") << MailCommon::SearchRule::Header;
    QTest::newRow("custom header") << QByteArray("X-DummyHeaderName") << MailCommon::SearchRule::Header;
    QTest::newRow("Organization") << QByteArray("Organization") << MailCommon::SearchRule::Header;
    QTest::newRow("List-Id") << QByteArray("List-Id") << MailCommon::SearchRule::Header;
    QTest::newRow("Resent-From") << QByteArray("Resent-From") << MailCommon::SearchRule::Header;
    QTest::newRow("X-Loop") << QByteArray("X-Loop") << MailCommon::SearchRule::Header;
    QTest::newRow("X-Mailing-List") << QByteArray("X-Mailing-List") << MailCommon::SearchRule::Header;
    QTest::newRow("X-Spam-Flag") << QByteArray("X-Spam-Flag") << MailCommon::SearchRule::Header;

    QTest::newRow("<message>") << QByteArray("<message>") << MailCommon::SearchRule::CompleteMessage;
    QTest::newRow("<body>") << QByteArray("<body>") << MailCommon::SearchRule::CompleteMessage;
}

void SearchPatternTest::shouldRuleRequirePart()
{
    QFETCH(QByteArray, field);
    QFETCH(MailCommon::SearchRule::RequiredPart, part);

    const MailCommon::SearchRule::Ptr rule = MailCommon::SearchRule::createInstance(field, MailCommon::SearchRule::FuncNone, QString());

    QCOMPARE(rule->requiredPart(), part);
}

QTEST_MAIN(SearchPatternTest)
