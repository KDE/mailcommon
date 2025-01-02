/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class InvalidFilterInfoWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit InvalidFilterInfoWidgetTest(QObject *parent = nullptr);
    ~InvalidFilterInfoWidgetTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
