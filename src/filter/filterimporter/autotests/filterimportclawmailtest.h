/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERIMPORTCLAWMAILTEST_H
#define FILTERIMPORTCLAWMAILTEST_H
#include <QObject>
class FilterImportClawMailtest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testImportFilters();
};

#endif // FILTERIMPORTCLAWMAILTEST_H
