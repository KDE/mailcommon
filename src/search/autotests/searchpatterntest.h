/*
  SPDX-FileCopyrightText: 2015 Tomas Trnka <tomastrnka@gmx.com>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class SearchPatternTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchPatternTest(QObject *parent = nullptr);
    ~SearchPatternTest() override;
private Q_SLOTS:
    void shouldRuleRequirePart_data();
    void shouldRuleRequirePart();
};

