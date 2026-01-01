/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class InvalidFilterWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit InvalidFilterWidgetTest(QObject *parent = nullptr);
    ~InvalidFilterWidgetTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
