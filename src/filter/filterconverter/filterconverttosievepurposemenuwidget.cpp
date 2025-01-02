/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "filterconverttosievepurposemenuwidget.h"
#include <TextCustomEditor/PlainTextEditor>
using namespace MailCommon;
FilterconverttosievePurposeMenuWidget::FilterconverttosievePurposeMenuWidget(QWidget *parentWidget, QObject *parent)
    : PimCommon::PurposeMenuWidget(parentWidget, parent)
{
}

FilterconverttosievePurposeMenuWidget::~FilterconverttosievePurposeMenuWidget() = default;

QByteArray FilterconverttosievePurposeMenuWidget::text()
{
    if (mEditor) {
        return mEditor->toPlainText().toUtf8();
    }
    return {};
}

void FilterconverttosievePurposeMenuWidget::setEditorWidget(TextCustomEditor::PlainTextEditor *editor)
{
    mEditor = editor;
}

#include "moc_filterconverttosievepurposemenuwidget.cpp"
