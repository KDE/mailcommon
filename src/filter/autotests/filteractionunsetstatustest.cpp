/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionunsetstatustest.h"
#include "../filteractions/filteractionunsetstatus.h"
#include <QTest>

FilterActionUnsetStatusTest::FilterActionUnsetStatusTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionUnsetStatusTest::~FilterActionUnsetStatusTest()
{
}

void FilterActionUnsetStatusTest::shouldHaveSieveRequires()
{
    MailCommon::FilterActionUnsetStatus filter;
    QCOMPARE(filter.sieveRequires(), QStringList() << QStringLiteral("imap4flags"));
}

void FilterActionUnsetStatusTest::shouldRequiresPart()
{
    MailCommon::FilterActionUnsetStatus filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::Envelope);
}

QTEST_MAIN(FilterActionUnsetStatusTest)
