/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

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
