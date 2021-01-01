/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "invalidfilterinfotest.h"
#include "../filter/invalidfilters/invalidfilterinfo.h"

#include <QTest>

InvalidFilterInfoTest::InvalidFilterInfoTest(QObject *parent)
    : QObject(parent)
{
}

InvalidFilterInfoTest::~InvalidFilterInfoTest()
{
}

void InvalidFilterInfoTest::shouldAddEmptyValue()
{
    MailCommon::InvalidFilterInfo info;
    QVERIFY(info.name().isEmpty());
    QVERIFY(info.information().isEmpty());
}

void InvalidFilterInfoTest::shouldAssignValueFromConstructor()
{
    const QString name = QStringLiteral("foo");
    const QString information = QStringLiteral("bla");
    MailCommon::InvalidFilterInfo info(name, information);
    QCOMPARE(info.name(), name);
    QCOMPARE(info.information(), information);
}

void InvalidFilterInfoTest::shouldAssignValue()
{
    MailCommon::InvalidFilterInfo info;
    const QString name = QStringLiteral("foo");
    const QString information = QStringLiteral("bla");
    info.setName(name);
    info.setInformation(information);
    QCOMPARE(info.name(), name);
    QCOMPARE(info.information(), information);
}

void InvalidFilterInfoTest::shouldBeEqual()
{
    MailCommon::InvalidFilterInfo info;
    const QString name = QStringLiteral("foo");
    const QString information = QStringLiteral("bla");
    info.setName(name);
    info.setInformation(information);
    MailCommon::InvalidFilterInfo copyInfo;
    copyInfo = info;
    QVERIFY(copyInfo == info);
}

QTEST_MAIN(InvalidFilterInfoTest)
