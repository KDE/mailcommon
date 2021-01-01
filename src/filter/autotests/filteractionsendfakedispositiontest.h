/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONSENDFAKEDISPOSITIONTEST_H
#define FILTERACTIONSENDFAKEDISPOSITIONTEST_H

#include <QObject>

class FilterActionSendFakeDispositionTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionSendFakeDispositionTest(QObject *parent = nullptr);
    ~FilterActionSendFakeDispositionTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldBeEmpty();
    void shouldHaveRequiredPart();
};

#endif // FILTERACTIONSENDFAKEDISPOSITIONTEST_H
