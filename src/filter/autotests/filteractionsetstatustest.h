/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONSETSTATUSTEST_H
#define FILTERACTIONSETSTATUSTEST_H

#include <QObject>

class FilterActionSetStatusTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionSetStatusTest(QObject *parent = nullptr);
    ~FilterActionSetStatusTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveSieveRequires();
    void shouldBeEmpty();
    void shouldRequiresPart();
};

#endif // FILTERACTIONSETSTATUSTEST_H
