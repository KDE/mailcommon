/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONDELETETEST_H
#define FILTERACTIONDELETETEST_H

#include <QObject>

class FilterActionDeleteTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionDeleteTest(QObject *parent = nullptr);
    ~FilterActionDeleteTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldReturnSieveValue();
    void shouldBeNotEmpty();
    void shouldRequiresPart();
    void shouldDeleteItem();
};

#endif // FILTERACTIONDELETETEST_H
