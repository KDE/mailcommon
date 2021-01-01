/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "filterconverttosievepurposemenuwidget.h"
#include <KPIMTextEdit/PlainTextEditor>

FilterconverttosievePurposeMenuWidget::FilterconverttosievePurposeMenuWidget(QWidget *parentWidget, QObject *parent)
    : PimCommon::PurposeMenuWidget(parentWidget, parent)
{
}

FilterconverttosievePurposeMenuWidget::~FilterconverttosievePurposeMenuWidget()
{
}

QByteArray FilterconverttosievePurposeMenuWidget::text()
{
    if (mEditor) {
        return mEditor->toPlainText().toUtf8();
    }
    return {};
}

void FilterconverttosievePurposeMenuWidget::setEditorWidget(KPIMTextEdit::PlainTextEditor *editor)
{
    mEditor = editor;
}
