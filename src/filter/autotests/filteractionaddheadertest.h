/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONADDHEADERTEST_H
#define FILTERACTIONADDHEADERTEST_H

#include <QObject>

class FilterActionAddHeaderTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionAddHeaderTest(QObject *parent = nullptr);
    ~FilterActionAddHeaderTest();
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

#endif // FILTERACTIONADDHEADERTEST_H
