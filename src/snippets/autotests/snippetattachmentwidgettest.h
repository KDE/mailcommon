/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SNIPPETATTACHMENTWIDGETTEST_H
#define SNIPPETATTACHMENTWIDGETTEST_H

#include <QObject>

class SnippetAttachmentWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SnippetAttachmentWidgetTest(QObject *parent = nullptr);
    ~SnippetAttachmentWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};

#endif // SNIPPETATTACHMENTWIDGETTEST_H
