/*
  SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterImporterPathCacheTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterImporterPathCacheTest(QObject *parent = nullptr);
    ~FilterImporterPathCacheTest() override;
private Q_SLOTS:
    void shouldReturnEmptyStringWhenListIsEmpty();
    void shouldNotStoreEmptyValue();
    void shouldNotDuplicateEntries();
    void shouldReturnValues();
};
