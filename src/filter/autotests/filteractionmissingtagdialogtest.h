/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONMISSINGTAGDIALOGTEST_H
#define FILTERACTIONMISSINGTAGDIALOGTEST_H

#include <QObject>

class FilterActionMissingTagDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionMissingTagDialogTest(QObject *parent = nullptr);
    ~FilterActionMissingTagDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // FILTERACTIONMISSINGTAGDIALOGTEST_H
