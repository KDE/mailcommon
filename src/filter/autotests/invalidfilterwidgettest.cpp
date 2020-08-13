/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "invalidfilterwidgettest.h"
#include "../filter/invalidfilters/invalidfilterwidget.h"
#include "../filter/invalidfilters/invalidfilterlistview.h"
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
    QLabel *lab = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(lab);

    MailCommon::InvalidFilterListView *list = w.findChild<MailCommon::InvalidFilterListView *>(QStringLiteral("invalidfilterlist"));
    QVERIFY(list);
}

QTEST_MAIN(InvalidFilterWidgetTest)
