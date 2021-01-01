/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONFORWARDTEST_H
#define FILTERACTIONFORWARDTEST_H

#include <QObject>

class FilterActionForwardTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionForwardTest(QObject *parent = nullptr);
    ~FilterActionForwardTest();
private Q_SLOTS:
    void shouldBeEmpty();
    void shouldRequiresPart();
};

#endif // FILTERACTIONFORWARDTEST_H
