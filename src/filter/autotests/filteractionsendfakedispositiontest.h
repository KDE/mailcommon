/*
  SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionSendFakeDispositionTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionSendFakeDispositionTest(QObject *parent = nullptr);
    ~FilterActionSendFakeDispositionTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldBeEmpty();
    void shouldHaveRequiredPart();
};
