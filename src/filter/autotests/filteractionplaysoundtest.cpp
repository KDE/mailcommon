/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionplaysoundtest.h"
#include "../filteractions/filteractionplaysound.h"
#include <QTest>
#include "filter/soundtestwidget.h"

FilterActionPlaySoundTest::FilterActionPlaySoundTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionPlaySoundTest::~FilterActionPlaySoundTest()
{
}

void FilterActionPlaySoundTest::shouldBeValid()
{
    MailCommon::FilterActionPlaySound filter;
    QVERIFY(filter.isEmpty());
    filter.argsFromString(QStringLiteral("foo"));
    QVERIFY(!filter.isEmpty());
}

void FilterActionPlaySoundTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionPlaySound filter;
    QWidget *w = filter.createParamWidget(nullptr);
    QVERIFY(w);
    QCOMPARE(w->objectName(), QStringLiteral("soundwidget"));
    MailCommon::SoundTestWidget *soundTest = dynamic_cast<MailCommon::SoundTestWidget *>(w);
    QVERIFY(soundTest);
    QVERIFY(soundTest->url().isEmpty());
}

void FilterActionPlaySoundTest::shouldHaveRequiredPart()
{
    MailCommon::FilterActionPlaySound filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::Envelope);
}

void FilterActionPlaySoundTest::shouldSieveRequres()
{
    MailCommon::FilterActionPlaySound w;
    QCOMPARE(w.sieveRequires(), QStringList());
}

QTEST_MAIN(FilterActionPlaySoundTest)
