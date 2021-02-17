/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SNIPPETWIDGETTEST_H
#define SNIPPETWIDGETTEST_H

#include <QObject>

class SnippetWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SnippetWidgetTest(QObject *parent = nullptr);
    ~SnippetWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldClearWidget();
};

#endif // SNIPPETWIDGETTEST_H
