/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef SNIPPETSMANAGERTEST_H
#define SNIPPETSMANAGERTEST_H

#include <QObject>

class SnippetsManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit SnippetsManagerTest(QObject *parent = nullptr);
    ~SnippetsManagerTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // SNIPPETSMANAGERTEST_H
