/*
  Copyright (c) 2017 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "filterimporterpathcachetest.h"
#include "../filterimporterpathcache.h"
#include <QTest>

QTEST_MAIN(FilterImporterPathCacheTest)

FilterImporterPathCacheTest::FilterImporterPathCacheTest(QObject *parent)
    : QObject(parent)
{

}

FilterImporterPathCacheTest::~FilterImporterPathCacheTest()
{

}

void FilterImporterPathCacheTest::shouldReturnEmptyStringWhenListIsEmpty()
{
    MailCommon::FilterImporterPathCache cache;
    QCOMPARE(cache.count(), 0);
    QVERIFY(cache.convertedFilterPath(QStringLiteral("dd")).isEmpty());
    QCOMPARE(cache.count(), 0);
}

void FilterImporterPathCacheTest::shouldNotStoreEmptyValue()
{
    MailCommon::FilterImporterPathCache cache;
    cache.insert(QString(), QStringLiteral("foo"));
    QCOMPARE(cache.count(), 0);

    cache.insert(QStringLiteral("foo"), QString());
    QCOMPARE(cache.count(), 0);

    cache.insert(QStringLiteral("foo1"), QStringLiteral("foo"));
    QCOMPARE(cache.count(), 1);
}

void FilterImporterPathCacheTest::shouldNotDuplicateEntries()
{
    MailCommon::FilterImporterPathCache cache;
    cache.insert(QStringLiteral("foo1"), QStringLiteral("foo"));
    QCOMPARE(cache.count(), 1);

    cache.insert(QStringLiteral("foo1"), QStringLiteral("foo"));
    QCOMPARE(cache.count(), 1);

    cache.insert(QStringLiteral("foo1"), QStringLiteral("foo"));
    QCOMPARE(cache.count(), 1);


    cache.insert(QStringLiteral("foo1"), QStringLiteral("foo2"));
    QCOMPARE(cache.count(), 1);

    cache.insert(QStringLiteral("foo1"), QStringLiteral("foo3"));
    QCOMPARE(cache.count(), 1);

    //Add new one
    cache.insert(QStringLiteral("foo2"), QStringLiteral("foo3"));
    QCOMPARE(cache.count(), 2);

}

void FilterImporterPathCacheTest::shouldReturnValues()
{
    MailCommon::FilterImporterPathCache cache;
    QString key = QStringLiteral("foo1");
    QString cached = QStringLiteral("foo");
    cache.insert(key, cached);
    QCOMPARE(cache.convertedFilterPath(key), cached);

    //Use value in same key
    cached = QStringLiteral("foo5");
    cache.insert(key, cached);
    QCOMPARE(cache.convertedFilterPath(key), cached);
}
