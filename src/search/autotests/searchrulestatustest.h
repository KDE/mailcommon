/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef SEARCHRULESTATUSTEST_H
#define SEARCHRULESTATUSTEST_H

#include <QObject>

class SearchRuleStatusTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchRuleStatusTest(QObject *parent = nullptr);
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveRequirePart();
    void shouldMatchStatus();
    void shouldMatchStatus_data();
    void shouldBeEmpty();
};

#endif // SEARCHRULESTATUSTEST_H
