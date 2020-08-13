/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionforwardtest.h"
#include <QTest>
#include "../filteractions/filteractionforward.h"

FilterActionForwardTest::FilterActionForwardTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionForwardTest::~FilterActionForwardTest()
{
}

void FilterActionForwardTest::shouldBeEmpty()
{
    MailCommon::FilterActionForward filter;
    QVERIFY(filter.isEmpty());
    filter.argsFromString(QStringLiteral("foo"));
    QVERIFY(!filter.isEmpty());
}

void FilterActionForwardTest::shouldRequiresPart()
{
    MailCommon::FilterActionForward filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

QTEST_MAIN(FilterActionForwardTest)
