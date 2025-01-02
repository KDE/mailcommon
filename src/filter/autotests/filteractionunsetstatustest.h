/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionUnsetStatusTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionUnsetStatusTest(QObject *parent = nullptr);
    ~FilterActionUnsetStatusTest() override;
private Q_SLOTS:
    void shouldHaveSieveRequires();
    void shouldRequiresPart();
};
