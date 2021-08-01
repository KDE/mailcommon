/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionRemoveHeaderTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionRemoveHeaderTest(QObject *parent = nullptr);
    ~FilterActionRemoveHeaderTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveSieveRequires();
    void shouldBeEmpty();
    void shouldNotExecuteActionWhenParameterIsEmpty();
    void shouldRemoveHeader();
    void shouldNotTryToRemoveHeaderWhenItDoesntExist();
    void shouldRemoveMultiHeader();
    void shouldHaveRequiredPart();
};

