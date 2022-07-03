/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KSyntaxHighlighting/Repository>
#include <QDialog>
namespace KPIMTextEdit
{
class PlainTextEditorWidget;
}

namespace MailCommon
{
class FilterConvertToSieveResultDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterConvertToSieveResultDialog(QWidget *parent = nullptr);
    ~FilterConvertToSieveResultDialog() override;

    void setCode(const QString &code);

private:
    void slotSave();
    void readConfig();
    void writeConfig();
    KPIMTextEdit::PlainTextEditorWidget *const mEditor;
    KSyntaxHighlighting::Repository mSyntaxRepo;
};
}
