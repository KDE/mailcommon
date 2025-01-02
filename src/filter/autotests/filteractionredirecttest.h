/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionRedirectTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionRedirectTest(QObject *parent = nullptr);
    ~FilterActionRedirectTest() override;
private Q_SLOTS:
    void shouldBeEmpty();
    void shouldHaveRequiredPart();
};
