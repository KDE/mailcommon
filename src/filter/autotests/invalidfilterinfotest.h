/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef INVALIDFILTERINFOTEST_H
#define INVALIDFILTERINFOTEST_H

#include <QObject>

class InvalidFilterInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit InvalidFilterInfoTest(QObject *parent = nullptr);
    ~InvalidFilterInfoTest();
private Q_SLOTS:
    void shouldAddEmptyValue();
    void shouldAssignValueFromConstructor();
    void shouldAssignValue();
    void shouldBeEqual();
};

#endif // INVALIDFILTERINFOTEST_H
