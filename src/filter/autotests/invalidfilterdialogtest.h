/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef INVALIDFILTERDIALOGTEST_H
#define INVALIDFILTERDIALOGTEST_H

#include <QObject>

class InvalidFilterDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit InvalidFilterDialogTest(QObject *parent = nullptr);
    ~InvalidFilterDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // INVALIDFILTERDIALOGTEST_H
