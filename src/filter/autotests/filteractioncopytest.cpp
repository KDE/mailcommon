/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractioncopytest.h"
#include "../filteractions/filteractioncopy.h"
#include <QTest>

FilterActionCopyTest::FilterActionCopyTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionCopyTest::~FilterActionCopyTest()
{
}

void FilterActionCopyTest::shouldBeEmpty()
{
    MailCommon::FilterActionCopy filter;
    QVERIFY(filter.isEmpty());
}

void FilterActionCopyTest::shouldHaveSieveRequires()
{
    MailCommon::FilterActionCopy filter;
    QCOMPARE(filter.sieveRequires(), QStringList() << QStringLiteral("fileinto") << QStringLiteral("copy"));
}

void FilterActionCopyTest::shouldHaveRequiresPart()
{
    MailCommon::FilterActionCopy filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::Envelope);
}

QTEST_MAIN(FilterActionCopyTest)
