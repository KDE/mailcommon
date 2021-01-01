/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef INVALIDFILTERINFOWIDGETTEST_H
#define INVALIDFILTERINFOWIDGETTEST_H

#include <QObject>

class InvalidFilterInfoWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit InvalidFilterInfoWidgetTest(QObject *parent = nullptr);
    ~InvalidFilterInfoWidgetTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // INVALIDFILTERINFOWIDGETTEST_H
