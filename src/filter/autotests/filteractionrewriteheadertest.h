/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONREWRITEHEADERTEST_H
#define FILTERACTIONREWRITEHEADERTEST_H

#include <QObject>

class FilterActionRewriteHeaderTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionRewriteHeaderTest(QObject *parent = nullptr);
    ~FilterActionRewriteHeaderTest();
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

#endif // FILTERACTIONREWRITEHEADERTEST_H
