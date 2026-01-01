/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class SearchRuleDateTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchRuleDateTest(QObject *parent = nullptr);

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldRequiresPart();
    void shouldBeEmpty();
    void shouldMatchDate();
    void shouldMatchDate_data();
};
