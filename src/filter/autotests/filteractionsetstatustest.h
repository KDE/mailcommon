/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionSetStatusTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionSetStatusTest(QObject *parent = nullptr);
    ~FilterActionSetStatusTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveSieveRequires();
    void shouldBeEmpty();
    void shouldRequiresPart();
};
