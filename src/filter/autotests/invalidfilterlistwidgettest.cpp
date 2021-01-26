/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "invalidfilterlistwidgettest.h"
#include "../filter/invalidfilters/invalidfilterinfo.h"
#include "../filter/invalidfilters/invalidfilterlistview.h"
#include <QTest>

InvalidFilterListWidgetTest::InvalidFilterListWidgetTest(QObject *parent)
    : QObject(parent)
{
}

InvalidFilterListWidgetTest::~InvalidFilterListWidgetTest()
{
}

void InvalidFilterListWidgetTest::shouldHaveDefaultValue()
{
    MailCommon::InvalidFilterListView w;
    QCOMPARE(w.model()->rowCount(), 0);
}

void InvalidFilterListWidgetTest::shouldAddInvalidFilters()
{
    MailCommon::InvalidFilterListView w;
    QVector<MailCommon::InvalidFilterInfo> lst;
    lst.append(MailCommon::InvalidFilterInfo(QStringLiteral("foo"), QStringLiteral("bla")));
    lst.append(MailCommon::InvalidFilterInfo(QStringLiteral("foo1"), QStringLiteral("bla1")));
    lst.append(MailCommon::InvalidFilterInfo(QStringLiteral("foo2"), QStringLiteral("bla2")));
    w.setInvalidFilters(lst);
    QCOMPARE(w.model()->rowCount(), 3);
}

QTEST_MAIN(InvalidFilterListWidgetTest)
