/*
  SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class MDNStateAttributeTest : public QObject
{
    Q_OBJECT
public:
    explicit MDNStateAttributeTest(QObject *parent = nullptr);
    ~MDNStateAttributeTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveType();
    void shouldSerializedAttribute();
    void shouldCloneAttribute();
};

