/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONWITHURLTEST_H
#define FILTERACTIONWITHURLTEST_H

#include <QObject>

class FilterActionWithUrlTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionWithUrlTest(QObject *parent = nullptr);
    ~FilterActionWithUrlTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldClearWidget();
    void shouldAddValue();
    void shouldApplyValue();
    void shouldTestUrl_data();
    void shouldTestUrl();
};

#endif // FILTERACTIONWITHURLTEST_H
