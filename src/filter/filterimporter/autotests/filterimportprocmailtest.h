/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERIMPORTPROCMAILTEST_H
#define FILTERIMPORTPROCMAILTEST_H
#include <QObject>
class FilterImportProcmailtest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testImportFilters();
};

#endif
