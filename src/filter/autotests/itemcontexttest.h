/*
  SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef ITEMCONTEXTTEST_H
#define ITEMCONTEXTTEST_H

#include <QObject>

class ItemContextTest : public QObject
{
    Q_OBJECT
public:
    explicit ItemContextTest(QObject *parent = nullptr);
    ~ItemContextTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignContext_data();
    void shouldAssignContext();
    void shouldAssignCollection();
};

#endif // ITEMCONTEXTTEST_H
