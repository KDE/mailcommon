/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONMISSINGIDENTITYDIALOGTEST_H
#define FILTERACTIONMISSINGIDENTITYDIALOGTEST_H

#include <QObject>

class FilterActionMissingIdentityDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionMissingIdentityDialogTest(QObject *parent = nullptr);
    ~FilterActionMissingIdentityDialogTest();
private Q_SLOTS:
    void initTestCase();
    void shouldHaveDefaultValue();
};

#endif // FILTERACTIONMISSINGIDENTITYDIALOGTEST_H
