/*
  SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionDeleteTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionDeleteTest(QObject *parent = nullptr);
    ~FilterActionDeleteTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldReturnSieveValue();
    void shouldBeNotEmpty();
    void shouldRequiresPart();
    void shouldDeleteItem();
};
