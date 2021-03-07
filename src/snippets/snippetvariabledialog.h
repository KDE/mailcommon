/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
  SPDX-FileContributor: Tobias Koenig <tokoe@kdab.com>

  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MAILCOMMON_SNIPPETVARIABLEDIALOG_P_H
#define MAILCOMMON_SNIPPETVARIABLEDIALOG_P_H

#include <QDialog>

class QCheckBox;
namespace KPIMTextEdit
{
class PlainTextEditorWidget;
}
namespace MailCommon
{
class SnippetVariableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SnippetVariableDialog(const QString &variableName, QMap<QString, QString> *variables, QWidget *parent = nullptr);
    ~SnippetVariableDialog() override;

    Q_REQUIRED_RESULT QString variableValue() const;
    Q_REQUIRED_RESULT bool saveVariableIsChecked() const;

private:
    void writeConfig();
    void readConfig();
    void slotAccepted();
    const QString mVariableName;
    QMap<QString, QString> *mVariables = nullptr;
    KPIMTextEdit::PlainTextEditorWidget *mVariableValueText = nullptr;
    QCheckBox *mSaveVariable = nullptr;
};
}

#endif
