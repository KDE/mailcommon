/*
    SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterLogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterLogTest(QObject *parent = nullptr);
    ~FilterLogTest() override = default;
private Q_SLOTS:
    void cleanup();
    void shouldHaveDefaultValues();
    void shouldNotAddLogEntries();
    void shouldAddLogEntries();
    void shouldAllowSpecificLogEntries();
    void shouldClearLog();
};
