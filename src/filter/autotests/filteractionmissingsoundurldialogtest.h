/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONMISSINGSOUNDURLDIALOGTEST_H
#define FILTERACTIONMISSINGSOUNDURLDIALOGTEST_H

#include <QObject>

class FilterActionMissingSoundUrlDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionMissingSoundUrlDialogTest(QObject *parent = nullptr);
    ~FilterActionMissingSoundUrlDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // FILTERACTIONMISSINGSOUNDURLDIALOGTEST_H
