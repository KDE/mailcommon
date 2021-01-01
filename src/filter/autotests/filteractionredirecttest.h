/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONREDIRECTTEST_H
#define FILTERACTIONREDIRECTTEST_H

#include <QObject>

class FilterActionRedirectTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionRedirectTest(QObject *parent = nullptr);
    ~FilterActionRedirectTest();
private Q_SLOTS:
    void shouldBeEmpty();
    void shouldHaveRequiredPart();
};

#endif // FILTERACTIONREDIRECTTEST_H
