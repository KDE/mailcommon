/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONMISSINGACCOUNTDIALOGTEST_H
#define FILTERACTIONMISSINGACCOUNTDIALOGTEST_H
#include <QObject>

class FilterActionMissingAccountDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionMissingAccountDialogTest(QObject *parent = nullptr);
    ~FilterActionMissingAccountDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // FILTERACTIONMISSINGACCOUNTDIALOGTEST_H
