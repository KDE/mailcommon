/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "invalidfilterwidgettest.h"
#include "../filter/invalidfilters/invalidfilterlistview.h"
#include "../filter/invalidfilters/invalidfilterwidget.h"
#include <QLabel>
#include <QTest>
InvalidFilterWidgetTest::InvalidFilterWidgetTest(QObject *parent)
    : QObject(parent)
{
}

InvalidFilterWidgetTest::~InvalidFilterWidgetTest()
{
}

void InvalidFilterWidgetTest::shouldHaveDefaultValue()
{
    MailCommon::InvalidFilterWidget w;
    auto *lab = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(lab);

    auto *list = w.findChild<MailCommon::InvalidFilterListView *>(QStringLiteral("invalidfilterlist"));
    QVERIFY(list);
}

QTEST_MAIN(InvalidFilterWidgetTest)
