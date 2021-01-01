/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERCONVERTTOSIEVERESULTDIALOGTEST_H
#define FILTERCONVERTTOSIEVERESULTDIALOGTEST_H

#include <QObject>

class FilterConvertToSieveResultDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterConvertToSieveResultDialogTest(QObject *parent = nullptr);
    ~FilterConvertToSieveResultDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAddCode();
};

#endif // FILTERCONVERTTOSIEVERESULTDIALOGTEST_H
