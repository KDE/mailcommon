/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERCONVERTTOSIEVEPURPOSEMENUWIDGET_H
#define FILTERCONVERTTOSIEVEPURPOSEMENUWIDGET_H

#include <PimCommon/PurposeMenuWidget>
namespace KPIMTextEdit
{
class PlainTextEditor;
}
class FilterconverttosievePurposeMenuWidget : public PimCommon::PurposeMenuWidget
{
    Q_OBJECT
public:
    explicit FilterconverttosievePurposeMenuWidget(QWidget *parentWidget, QObject *parent = nullptr);
    ~FilterconverttosievePurposeMenuWidget() override;

    QByteArray text() override;
    void setEditorWidget(KPIMTextEdit::PlainTextEditor *editor);

private:
    KPIMTextEdit::PlainTextEditor *mEditor = nullptr;
};

#endif // FILTERCONVERTTOSIEVEPURPOSEMENUWIDGET_H
