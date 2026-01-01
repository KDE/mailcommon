/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

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
