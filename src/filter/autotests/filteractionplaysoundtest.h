/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#ifndef FILTERACTIONPLAYSOUNDTEST_H
#define FILTERACTIONPLAYSOUNDTEST_H

#include <QObject>

class FilterActionPlaySoundTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionPlaySoundTest(QObject *parent = nullptr);
    ~FilterActionPlaySoundTest();
private Q_SLOTS:
    void shouldBeValid();
    void shouldHaveDefaultValue();
    void shouldHaveRequiredPart();
    void shouldSieveRequres();
};

#endif // FILTERACTIONPLAYSOUNDTEST_H
