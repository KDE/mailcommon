/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SNIPPETSELECTATTACHMENTDIALOGTEST_H
#define SNIPPETSELECTATTACHMENTDIALOGTEST_H

#include <QObject>

class SnippetSelectAttachmentDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit SnippetSelectAttachmentDialogTest(QObject *parent = nullptr);
    ~SnippetSelectAttachmentDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

#endif // SNIPPETSELECTATTACHMENTDIALOGTEST_H
