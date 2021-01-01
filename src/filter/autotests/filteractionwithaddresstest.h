/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONWITHADDRESSTEST_H
#define FILTERACTIONWITHADDRESSTEST_H

#include <QObject>

class FilterActionWithAddressTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionWithAddressTest(QObject *parent = nullptr);
    ~FilterActionWithAddressTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
    void shouldRequiresPart();
};

#endif // FILTERACTIONWITHADDRESSTEST_H
