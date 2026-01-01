/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class FilterActionMissingTransportDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionMissingTransportDialogTest(QObject *parent = nullptr);
    ~FilterActionMissingTransportDialogTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
