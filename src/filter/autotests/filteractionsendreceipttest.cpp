/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#include "filteractionsendreceipttest.h"
#include "../filteractions/filteractionsendreceipt.h"
#include <QTest>

FilterActionSendReceiptTest::FilterActionSendReceiptTest(QObject *parent)
    : QObject(parent)
{
}

void FilterActionSendReceiptTest::shouldBeNotEmpty()
{
    MailCommon::FilterActionSendReceipt filter;
    QVERIFY(!filter.isEmpty());
}

void FilterActionSendReceiptTest::shouldRequiresPart()
{
    MailCommon::FilterActionSendReceipt filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

void FilterActionSendReceiptTest::shouldSieveRequires()
{
    MailCommon::FilterActionSendReceipt filter;
    QCOMPARE(filter.sieveRequires(), QStringList());
}

QTEST_MAIN(FilterActionSendReceiptTest)
