/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "snoozeattributetest.h"
#include "attributes/snoozeattribute.h"
#include <QTest>
QTEST_GUILESS_MAIN(SnoozeAttributeTest)
using namespace Qt::Literals::StringLiterals;
SnoozeAttributeTest::SnoozeAttributeTest(QObject *parent)
    : QObject{parent}
{
}

void SnoozeAttributeTest::shouldHaveDefaultValues()
{
    const MailCommon::SnoozeAttribute attr;
    QVERIFY(!attr.wakeUpDateTime().isValid());
    QCOMPARE(attr.type(), "SnoozeAttribute"_ba);
}

void SnoozeAttributeTest::shouldCloneAttributes()
{
    MailCommon::SnoozeAttribute attr;
    attr.setWakeUpDateTime(QDateTime(QDate(2026, 9, 9), QTime(8, 8, 8)));

    MailCommon::SnoozeAttribute *result = attr.clone();
    QCOMPARE(attr, *result);
    delete result;
}

#include "moc_snoozeattributetest.cpp"
