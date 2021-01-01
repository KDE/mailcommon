/*
  SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef EXPIRECOLLECTIONATTRIBUTETEST_H
#define EXPIRECOLLECTIONATTRIBUTETEST_H

#include <QObject>

class ExpireCollectionAttributeTest : public QObject
{
    Q_OBJECT
public:
    explicit ExpireCollectionAttributeTest(QObject *parent = nullptr);
    ~ExpireCollectionAttributeTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue_data();
    void shouldAssignValue();
    void shouldCloneAttr_data();
    void shouldCloneAttr();
    void shouldSerializedValue_data();
    void shouldSerializedValue();
    void shouldHaveType();
};

#endif // EXPIRECOLLECTIONATTRIBUTETEST_H
