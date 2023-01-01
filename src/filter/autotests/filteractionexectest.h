/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionExecTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionExecTest(QObject *parent = nullptr);
    ~FilterActionExecTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveRequirePart();
};
