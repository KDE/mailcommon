/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef SEARCHRULENUMERICALTEST_H
#define SEARCHRULENUMERICALTEST_H

#include <QObject>

class SearchRuleNumericalTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchRuleNumericalTest(QObject *parent = nullptr);

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldBeEmpty();
    void shouldHaveRequirePart();
    void shouldMatchNumericalsize();
    void shouldMatchNumericalsize_data();
};

#endif // SEARCHRULENUMERICALTEST_H
