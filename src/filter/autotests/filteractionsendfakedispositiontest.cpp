/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionsendfakedispositiontest.h"
#include "../filteractions/filteractionsendfakedisposition.h"
#include <QComboBox>
#include <QTest>

FilterActionSendFakeDispositionTest::FilterActionSendFakeDispositionTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionSendFakeDispositionTest::~FilterActionSendFakeDispositionTest()
{
}

void FilterActionSendFakeDispositionTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionSendFakeDisposition filter;
    QWidget *w = filter.createParamWidget(nullptr);
    QCOMPARE(w->objectName(), QStringLiteral("combobox"));

    auto comboBox = dynamic_cast<QComboBox *>(w);
    QVERIFY(comboBox);
    QVERIFY(!comboBox->isEditable());
    QVERIFY(comboBox->count() > 0);
}

void FilterActionSendFakeDispositionTest::shouldBeEmpty()
{
    MailCommon::FilterActionSendFakeDisposition filter;
    QVERIFY(filter.isEmpty());
}

void FilterActionSendFakeDispositionTest::shouldHaveRequiredPart()
{
    MailCommon::FilterActionSendFakeDisposition filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

QTEST_MAIN(FilterActionSendFakeDispositionTest)
