/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONUNSETSTATUSTEST_H
#define FILTERACTIONUNSETSTATUSTEST_H

#include <QObject>

class FilterActionUnsetStatusTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionUnsetStatusTest(QObject *parent = nullptr);
    ~FilterActionUnsetStatusTest();
private Q_SLOTS:
    void shouldHaveSieveRequires();
    void shouldRequiresPart();
};

#endif // FILTERACTIONUNSETSTATUSTEST_H
