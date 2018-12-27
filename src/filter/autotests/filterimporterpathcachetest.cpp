/*
  Copyright (C) 2017-2019 Montel Laurent <montel@kde.org>

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
#include <AkonadiCore/Collection>
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
    QVERIFY(!cache.convertedFilterPath(QStringLiteral("foo")).isValid());
    QCOMPARE(cache.count(), 0);
}

void FilterImporterPathCacheTest::shouldNotStoreEmptyValue()
{
    MailCommon::FilterImporterPathCache cache;
    cache.insert(QString(), Akonadi::Collection(3));
    QCOMPARE(cache.count(), 0);

    cache.insert(QStringLiteral("foo"), Akonadi::Collection(-1));
    QCOMPARE(cache.count(), 0);

    cache.insert(QStringLiteral("foo1"), Akonadi::Collection(3));
    QCOMPARE(cache.count(), 1);
}

void FilterImporterPathCacheTest::shouldNotDuplicateEntries()
{
    MailCommon::FilterImporterPathCache cache;
    cache.insert(QStringLiteral("foo1"), Akonadi::Collection(3));
    QCOMPARE(cache.count(), 1);

    cache.insert(QStringLiteral("foo1"), Akonadi::Collection(3));
    QCOMPARE(cache.count(), 1);

    cache.insert(QStringLiteral("foo1"), Akonadi::Collection(3));
    QCOMPARE(cache.count(), 1);

    cache.insert(QStringLiteral("foo1"), Akonadi::Collection(4));
    QCOMPARE(cache.count(), 1);

    cache.insert(QStringLiteral("foo1"), Akonadi::Collection(4));
    QCOMPARE(cache.count(), 1);

    //Add new one
    cache.insert(QStringLiteral("foo2"), Akonadi::Collection(4));
    QCOMPARE(cache.count(), 2);
}

void FilterImporterPathCacheTest::shouldReturnValues()
{
    MailCommon::FilterImporterPathCache cache;
    QString key = QStringLiteral("foo1");
    Akonadi::Collection cached = Akonadi::Collection(3);
    cache.insert(key, cached);
    QCOMPARE(cache.convertedFilterPath(key), cached);

    //Use value in same key
    cached = Akonadi::Collection(5);
    cache.insert(key, cached);
    QCOMPARE(cache.convertedFilterPath(key), cached);
}
