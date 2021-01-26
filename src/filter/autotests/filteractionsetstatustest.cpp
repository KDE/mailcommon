/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionsetstatustest.h"
#include "../filteractions/filteractionsetstatus.h"
#include <QComboBox>
#include <QTest>
#include <QWidget>

FilterActionSetStatusTest::FilterActionSetStatusTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionSetStatusTest::~FilterActionSetStatusTest()
{
}

void FilterActionSetStatusTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionSetStatus filter;
    QWidget *w = filter.createParamWidget(nullptr);
    QCOMPARE(w->objectName(), QStringLiteral("combobox"));
    auto comboBox = dynamic_cast<QComboBox *>(w);
    QVERIFY(comboBox);
    QVERIFY(!comboBox->isEditable());
    QVERIFY(comboBox->count() > 0);
}

void FilterActionSetStatusTest::shouldHaveSieveRequires()
{
    MailCommon::FilterActionSetStatus filter;
    QCOMPARE(filter.sieveRequires(), QStringList() << QStringLiteral("imap4flags"));
}

void FilterActionSetStatusTest::shouldBeEmpty()
{
    MailCommon::FilterActionSetStatus filter;
    QVERIFY(filter.isEmpty());
}

void FilterActionSetStatusTest::shouldRequiresPart()
{
    MailCommon::FilterActionSetStatus filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::Envelope);
}

QTEST_MAIN(FilterActionSetStatusTest)
