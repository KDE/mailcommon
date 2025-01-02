/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionForwardTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionForwardTest(QObject *parent = nullptr);
    ~FilterActionForwardTest() override;
private Q_SLOTS:
    void shouldBeEmpty();
    void shouldRequiresPart();
};
