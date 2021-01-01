/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONMISSINGTEMPLATEDIALOGTEST_H
#define FILTERACTIONMISSINGTEMPLATEDIALOGTEST_H

#include <QObject>

class FilterActionMissingTemplateDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionMissingTemplateDialogTest(QObject *parent = nullptr);
    ~FilterActionMissingTemplateDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // FILTERACTIONMISSINGTEMPLATEDIALOGTEST_H
