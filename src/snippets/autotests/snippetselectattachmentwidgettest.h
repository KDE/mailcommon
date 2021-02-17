/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SNIPPETSELECTATTACHMENTWIDGETTEST_H
#define SNIPPETSELECTATTACHMENTWIDGETTEST_H

#include <QObject>

class SnippetSelectAttachmentWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SnippetSelectAttachmentWidgetTest(QObject *parent = nullptr);
    ~SnippetSelectAttachmentWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

#endif // SNIPPETSELECTATTACHMENTWIDGETTEST_H
