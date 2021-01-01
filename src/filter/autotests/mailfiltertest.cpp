/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "mailfiltertest.h"
#include "../mailfilter.h"
#include <QTest>

MailFilterTest::MailFilterTest(QObject *parent)
    : QObject(parent)
{
}

MailFilterTest::~MailFilterTest()
{
}

void MailFilterTest::shouldHaveDefaultValue()
{
    MailCommon::MailFilter mailfilter;
    QVERIFY(mailfilter.isEmpty());
    QVERIFY(mailfilter.isEnabled());
    QVERIFY(mailfilter.applyOnInbound());
    QVERIFY(!mailfilter.applyBeforeOutbound());
    QVERIFY(mailfilter.applyOnExplicit());
    QVERIFY(mailfilter.stopProcessingHere());
    QVERIFY(!mailfilter.configureShortcut());
    QVERIFY(!mailfilter.configureToolbar());
    QVERIFY(mailfilter.isAutoNaming());
    QCOMPARE(mailfilter.applicability(), MailCommon::MailFilter::All);
    QVERIFY(mailfilter.actions()->isEmpty());
    QVERIFY(mailfilter.icon().isEmpty());
}

void MailFilterTest::shouldApplySettings()
{
    MailCommon::MailFilter mailfilter;
    bool value = true;
    mailfilter.setApplyBeforeOutbound(value);
    QCOMPARE(mailfilter.applyBeforeOutbound(), value);
    value = false;
    mailfilter.setApplyBeforeOutbound(value);
    QCOMPARE(mailfilter.applyBeforeOutbound(), value);
}

QTEST_MAIN(MailFilterTest)
