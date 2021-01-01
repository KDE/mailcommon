/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONEXECTEST_H
#define FILTERACTIONEXECTEST_H

#include <QObject>

class FilterActionExecTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionExecTest(QObject *parent = nullptr);
    ~FilterActionExecTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveRequirePart();
};

#endif // FILTERACTIONEXECTEST_H
