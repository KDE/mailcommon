/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#pragma once

#include <QObject>

class SearchRuleStringTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchRuleStringTest(QObject *parent = nullptr);

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldMatchString();
    void shouldHaveRequirePart_data();
    void shouldHaveRequirePart();
    void shouldMatchString_data();
    void shouldBeEmpty();
};
