/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef SEARCHRULETEST_H
#define SEARCHRULETEST_H

#include <QObject>

class SearchRuleTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchRuleTest(QObject *parent = nullptr);
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
};

#endif // SEARCHRULETEST_H
