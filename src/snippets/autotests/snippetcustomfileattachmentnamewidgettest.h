/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SNIPPETCUSTOMFILEATTACHMENTNAMEWIDGETTEST_H
#define SNIPPETCUSTOMFILEATTACHMENTNAMEWIDGETTEST_H

#include <QObject>

class SnippetCustomFileAttachmentNameWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SnippetCustomFileAttachmentNameWidgetTest(QObject *parent = nullptr);
    ~SnippetCustomFileAttachmentNameWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

#endif // SNIPPETCUSTOMFILEATTACHMENTNAMEWIDGETTEST_H
