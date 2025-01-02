/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include <QObject>
class FilterImportThunderbirdtest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testImportFiltersAllCondition();
    void testImportFiltersEmpty();
    void testImportFiltersStopExecution();
    void testImportFiltersDisabled();
    void testImportTwoFilters();
    void initTestCase();
    void testImportAndFilters();
    void testImportOrFilters();
    void testImportTypeFilters();
};
