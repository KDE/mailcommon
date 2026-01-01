/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include <QObject>
class FilterImportSylpheedtest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testImportFilters();
};
