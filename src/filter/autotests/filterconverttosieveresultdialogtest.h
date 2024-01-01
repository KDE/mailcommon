/*
  SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterConvertToSieveResultDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterConvertToSieveResultDialogTest(QObject *parent = nullptr);
    ~FilterConvertToSieveResultDialogTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAddCode();
};
