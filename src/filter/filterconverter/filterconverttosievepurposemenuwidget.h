/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <PimCommon/PurposeMenuWidget>
namespace TextCustomEditor
{
class PlainTextEditor;
}
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT FilterconverttosievePurposeMenuWidget : public PimCommon::PurposeMenuWidget
{
    Q_OBJECT
public:
    explicit FilterconverttosievePurposeMenuWidget(QWidget *parentWidget, QObject *parent = nullptr);
    ~FilterconverttosievePurposeMenuWidget() override;

    [[nodiscard]] QByteArray text() override;
    void setEditorWidget(TextCustomEditor::PlainTextEditor *editor);

private:
    TextCustomEditor::PlainTextEditor *mEditor = nullptr;
};
}
