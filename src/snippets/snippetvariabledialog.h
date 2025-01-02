/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
  SPDX-FileContributor: Tobias Koenig <tokoe@kdab.com>

  SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

class QCheckBox;
namespace TextCustomEditor
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

    [[nodiscard]] QString variableValue() const;
    [[nodiscard]] bool saveVariableIsChecked() const;

private:
    void writeConfig();
    void readConfig();
    void slotAccepted();
    const QString mVariableName;
    QMap<QString, QString> *mVariables = nullptr;
    TextCustomEditor::PlainTextEditorWidget *const mVariableValueText;
    QCheckBox *mSaveVariable = nullptr;
};
}
