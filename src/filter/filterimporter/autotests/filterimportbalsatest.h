/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include <QObject>
class FilterImportBalsatest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testImportFilters();
};
