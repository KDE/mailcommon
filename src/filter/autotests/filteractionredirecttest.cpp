/*
  SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionredirecttest.h"
#include "../filteractions/filteractionredirect.h"
#include <QTest>

FilterActionRedirectTest::FilterActionRedirectTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionRedirectTest::~FilterActionRedirectTest() = default;

void FilterActionRedirectTest::shouldBeEmpty()
{
    MailCommon::FilterActionRedirect filter;
    QVERIFY(filter.isEmpty());
}

void FilterActionRedirectTest::shouldHaveRequiredPart()
{
    MailCommon::FilterActionRedirect filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

QTEST_MAIN(FilterActionRedirectTest)

#include "moc_filteractionredirecttest.cpp"
