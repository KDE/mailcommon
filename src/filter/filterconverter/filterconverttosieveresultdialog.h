/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FILTERCONVERTTOSIEVERESULTDIALOG_H
#define FILTERCONVERTTOSIEVERESULTDIALOG_H

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
    ~FilterConvertToSieveResultDialog();

    void setCode(const QString &code);

private Q_SLOTS:
    void slotSave();

private:
    void readConfig();
    void writeConfig();
    KPIMTextEdit::PlainTextEditorWidget *mEditor = nullptr;
    KSyntaxHighlighting::Repository mSyntaxRepo;
};
}

#endif // FILTERCONVERTTOSIEVERESULTDIALOG_H
