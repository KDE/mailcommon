/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERIMPORTERGMAILTEST_H
#define FILTERIMPORTERGMAILTEST_H

#include <QObject>

class FilterImporterGmailTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterImporterGmailTest(QObject *parent = nullptr);
private Q_SLOTS:
    void initTestCase();
    void testImportFilters();
};

#endif // FILTERIMPORTERGMAILTEST_H
