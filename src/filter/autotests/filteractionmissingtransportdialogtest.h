/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONMISSINGTRANSPORTDIALOGTEST_H
#define FILTERACTIONMISSINGTRANSPORTDIALOGTEST_H

#include <QObject>

class FilterActionMissingTransportDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionMissingTransportDialogTest(QObject *parent = nullptr);
    ~FilterActionMissingTransportDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // FILTERACTIONMISSINGTRANSPORTDIALOGTEST_H
