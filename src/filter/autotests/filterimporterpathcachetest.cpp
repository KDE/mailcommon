/*
  SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
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

    // Add new one
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

    // Use value in same key
    cached = Akonadi::Collection(5);
    cache.insert(key, cached);
    QCOMPARE(cache.convertedFilterPath(key), cached);
}
