/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

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
