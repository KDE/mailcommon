/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionAddHeaderTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionAddHeaderTest(QObject *parent = nullptr);
    ~FilterActionAddHeaderTest() override;
private Q_SLOTS:
    void shouldCreateWidget();
    void shouldAddValue();
    void shouldAddValue_data();
    void shouldClearWidget();
    void shouldReturnSieveCode();
    void shouldBeEmpty();

    void shouldNotExecuteActionWhenParameterIsEmpty();
    void shouldNotExecuteActionWhenValueIsEmpty();
    void shouldAddNewHeaderWhenNotExistingHeader();
    void shouldReplaceHeaderWhenExistingHeader();
    void shouldRequiresSieve();
};
