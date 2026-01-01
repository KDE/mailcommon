/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "invalidfilterinfowidgettest.h"
#include "../invalidfilters/invalidfilterinfowidget.h"
#include <QTest>

InvalidFilterInfoWidgetTest::InvalidFilterInfoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

InvalidFilterInfoWidgetTest::~InvalidFilterInfoWidgetTest() = default;

void InvalidFilterInfoWidgetTest::shouldHaveDefaultValue()
{
    MailCommon::InvalidFilterInfoWidget w;
    QVERIFY(!w.isVisible());
    QVERIFY(!w.isCloseButtonVisible());
}

QTEST_MAIN(InvalidFilterInfoWidgetTest)

#include "moc_invalidfilterinfowidgettest.cpp"
