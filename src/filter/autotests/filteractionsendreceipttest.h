/*
  SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#pragma once

#include <QObject>

class FilterActionSendReceiptTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionSendReceiptTest(QObject *parent = nullptr);
private Q_SLOTS:
    void shouldBeNotEmpty();
    void shouldRequiresPart();
    void shouldSieveRequires();
};
