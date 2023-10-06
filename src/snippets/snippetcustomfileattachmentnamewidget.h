/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <MessageComposer/ConvertSnippetVariablesUtil>
#include <QWidget>
class QLineEdit;
namespace MessageComposer
{
class ConvertSnippetVariableMenu;
}

namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT SnippetCustomFileAttachmentNameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SnippetCustomFileAttachmentNameWidget(QWidget *parent = nullptr);
    ~SnippetCustomFileAttachmentNameWidget() override;
    [[nodiscard]] QString result() const;
    void setText(const QString &str);

private:
    void insertVariable(MessageComposer::ConvertSnippetVariablesUtil::VariableType variable);
    QLineEdit *const mLineEdit;
    MessageComposer::ConvertSnippetVariableMenu *mConvertMenu = nullptr;
};
}
