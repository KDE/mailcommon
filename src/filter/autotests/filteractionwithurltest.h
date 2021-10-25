/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionWithUrlTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionWithUrlTest(QObject *parent = nullptr);
    ~FilterActionWithUrlTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldClearWidget();
    void shouldAddValue();
    void shouldApplyValue();
    void shouldTestUrl_data();
    void shouldTestUrl();
};

