/*
    SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#include "filterlogtest.h"
#include "filter/filterlog.h"
#include <QStandardPaths>
#include <QTest>

QTEST_MAIN(FilterLogTest)
FilterLogTest::FilterLogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void FilterLogTest::cleanup()
{
    auto log = MailCommon::FilterLog::instance();
    log->clear();
    log->setLogging(false);
    log->setContentTypeEnabled(MailCommon::FilterLog::Meta, true);
    log->setContentTypeEnabled(MailCommon::FilterLog::PatternDescription, true);
    log->setContentTypeEnabled(MailCommon::FilterLog::RuleResult, true);
    log->setContentTypeEnabled(MailCommon::FilterLog::PatternResult, true);
    log->setContentTypeEnabled(MailCommon::FilterLog::AppliedAction, true);
}

void FilterLogTest::shouldHaveDefaultValues()
{
    auto log = MailCommon::FilterLog::instance();
    QVERIFY(!log->isLogging());
    constexpr long size = 512 * 1024;
    QCOMPARE(log->maxLogSize(), size);
    QVERIFY(log->logEntries().isEmpty());
    QVERIFY(log->isContentTypeEnabled(MailCommon::FilterLog::Meta));
    QVERIFY(log->isContentTypeEnabled(MailCommon::FilterLog::PatternDescription));
    QVERIFY(log->isContentTypeEnabled(MailCommon::FilterLog::RuleResult));
    QVERIFY(log->isContentTypeEnabled(MailCommon::FilterLog::PatternResult));
    QVERIFY(log->isContentTypeEnabled(MailCommon::FilterLog::AppliedAction));
}

void FilterLogTest::shouldNotAddLogEntries()
{
    auto log = MailCommon::FilterLog::instance();
    log->setLogging(false);
    log->add(QStringLiteral("foo"), MailCommon::FilterLog::AppliedAction);
    QVERIFY(log->logEntries().isEmpty());

    log->setLogging(true);
    log->add(QStringLiteral("foo"), MailCommon::FilterLog::AppliedAction);
    QCOMPARE(log->logEntries().size(), 1);
}

void FilterLogTest::shouldAddLogEntries()
{
    auto log = MailCommon::FilterLog::instance();
    log->setLogging(true);
    log->add(QStringLiteral("foo"), MailCommon::FilterLog::AppliedAction);
    QCOMPARE(log->logEntries().size(), 1);

    log->add(QStringLiteral("foo"), MailCommon::FilterLog::AppliedAction);
    QCOMPARE(log->logEntries().size(), 2);
    log->add(QStringLiteral("foo"), MailCommon::FilterLog::AppliedAction);
    QCOMPARE(log->logEntries().size(), 3);
}

void FilterLogTest::shouldAllowSpecificLogEntries()
{
    auto log = MailCommon::FilterLog::instance();
    log->setLogging(true);
    log->setContentTypeEnabled(MailCommon::FilterLog::Meta, true);
    log->setContentTypeEnabled(MailCommon::FilterLog::PatternDescription, false);
    log->setContentTypeEnabled(MailCommon::FilterLog::RuleResult, false);
    log->setContentTypeEnabled(MailCommon::FilterLog::PatternResult, false);
    log->setContentTypeEnabled(MailCommon::FilterLog::AppliedAction, false);
    log->add(QStringLiteral("foo"), MailCommon::FilterLog::AppliedAction);
    QCOMPARE(log->logEntries().size(), 0);

    log->add(QStringLiteral("foo"), MailCommon::FilterLog::Meta);
    QCOMPARE(log->logEntries().size(), 1);

    log->add(QStringLiteral("foo"), MailCommon::FilterLog::PatternDescription);
    QCOMPARE(log->logEntries().size(), 1);

    log->add(QStringLiteral("foo"), MailCommon::FilterLog::RuleResult);
    QCOMPARE(log->logEntries().size(), 1);

    log->add(QStringLiteral("foo"), MailCommon::FilterLog::PatternResult);
    QCOMPARE(log->logEntries().size(), 1);

    log->add(QStringLiteral("foo"), MailCommon::FilterLog::AppliedAction);
    QCOMPARE(log->logEntries().size(), 1);

    log->add(QStringLiteral("foo"), MailCommon::FilterLog::Meta);
    QCOMPARE(log->logEntries().size(), 2);

    log->setContentTypeEnabled(MailCommon::FilterLog::PatternResult, true);
    log->add(QStringLiteral("foo"), MailCommon::FilterLog::Meta);
    QCOMPARE(log->logEntries().size(), 3);

    log->add(QStringLiteral("foo"), MailCommon::FilterLog::PatternResult);
    QCOMPARE(log->logEntries().size(), 4);
}

void FilterLogTest::shouldClearLog()
{
    auto log = MailCommon::FilterLog::instance();
    log->setLogging(true);
    log->add(QStringLiteral("foo"), MailCommon::FilterLog::Meta);
    QCOMPARE(log->logEntries().size(), 1);

    log->clear();
    QCOMPARE(log->logEntries().size(), 0);
}

#include "moc_filterlogtest.cpp"
