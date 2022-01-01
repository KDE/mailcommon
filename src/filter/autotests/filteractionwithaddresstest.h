/*
  SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionWithAddressTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionWithAddressTest(QObject *parent = nullptr);
    ~FilterActionWithAddressTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
    void shouldRequiresPart();
};

