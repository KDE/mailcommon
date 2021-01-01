/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONCOPYTEST_H
#define FILTERACTIONCOPYTEST_H

#include <QObject>

class FilterActionCopyTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionCopyTest(QObject *parent = nullptr);
    ~FilterActionCopyTest();
private Q_SLOTS:
    void shouldBeEmpty();
    void shouldHaveSieveRequires();
    void shouldHaveRequiresPart();
};

#endif // FILTERACTIONCOPYTEST_H
