/*
  SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#pragma once

#include <QObject>

class FilterActionPlaySoundTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionPlaySoundTest(QObject *parent = nullptr);
    ~FilterActionPlaySoundTest() override;
private Q_SLOTS:
    void shouldBeValid();
    void shouldHaveDefaultValue();
    void shouldHaveRequiredPart();
    void shouldSieveRequres();
};
