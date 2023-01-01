/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionRewriteHeaderTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionRewriteHeaderTest(QObject *parent = nullptr);
    ~FilterActionRewriteHeaderTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldBeEmpty();
    void shouldNotExecuteActionWhenParameterIsEmpty();
    void shouldNotExecuteActionWhenValueIsEmpty();
    void shouldRewriteHeader();
    void shouldNotRewriteHeaderWhenHeaderNotFound();
    void shouldNotRewriteHeaderWhenRegexpNotFound();
    void shouldHaveRequiredPart();
};
