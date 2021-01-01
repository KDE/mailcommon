/*
  SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERIMPORTERPATHCACHETEST_H
#define FILTERIMPORTERPATHCACHETEST_H

#include <QObject>

class FilterImporterPathCacheTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterImporterPathCacheTest(QObject *parent = nullptr);
    ~FilterImporterPathCacheTest();
private Q_SLOTS:
    void shouldReturnEmptyStringWhenListIsEmpty();
    void shouldNotStoreEmptyValue();
    void shouldNotDuplicateEntries();
    void shouldReturnValues();
};

#endif // FILTERIMPORTERPATHCACHETEST_H
