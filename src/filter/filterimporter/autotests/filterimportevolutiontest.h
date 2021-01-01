/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERIMPORTEVOLUTIONTEST_H
#define FILTERIMPORTEVOLUTIONTEST_H
#include <QObject>
class FilterImportEvolutiontest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testImportFilters();
};

#endif // FILTERIMPORTEVOLUTIONTEST_H
