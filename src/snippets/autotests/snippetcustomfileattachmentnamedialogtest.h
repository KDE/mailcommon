/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SNIPPETCUSTOMFILEATTACHMENTNAMEDIALOGTEST_H
#define SNIPPETCUSTOMFILEATTACHMENTNAMEDIALOGTEST_H

#include <QObject>

class SnippetCustomFileAttachmentNameDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit SnippetCustomFileAttachmentNameDialogTest(QObject *parent = nullptr);
    ~SnippetCustomFileAttachmentNameDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

#endif // SNIPPETCUSTOMFILEATTACHMENTNAMEDIALOGTEST_H
