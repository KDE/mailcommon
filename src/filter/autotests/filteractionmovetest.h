/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionMoveTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionMoveTest(QObject *parent = nullptr);
    ~FilterActionMoveTest() override;
private Q_SLOTS:
    void shouldHaveSieveRequires();
    void shouldHaveRequiresPart();
    void shouldHaveDefaultValue();
};
