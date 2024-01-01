/*
  SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionCopyTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionCopyTest(QObject *parent = nullptr);
    ~FilterActionCopyTest() override;
private Q_SLOTS:
    void shouldBeEmpty();
    void shouldHaveSieveRequires();
    void shouldHaveRequiresPart();
};
