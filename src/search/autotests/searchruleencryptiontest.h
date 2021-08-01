/*
  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class SearchRuleEncryptionTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void shouldRequiresPart();
    void shouldNotBeEmpty();
    void shouldMatchMessage();
    void shouldMatchMessage_data();
};

