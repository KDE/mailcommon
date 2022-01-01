/*
  SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class SnippetsManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit SnippetsManagerTest(QObject *parent = nullptr);
    ~SnippetsManagerTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

