/*
   SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SNIPPETCUSTOMFILEATTACHMENTNAMEWIDGET_H
#define SNIPPETCUSTOMFILEATTACHMENTNAMEWIDGET_H

#include <QWidget>
#include "mailcommon_private_export.h"
#include <MessageComposer/ConvertSnippetVariablesUtil>
class QLineEdit;
namespace MessageComposer {
class ConvertSnippetVariableMenu;
}

namespace MailCommon {
class MAILCOMMON_TESTS_EXPORT SnippetCustomFileAttachmentNameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SnippetCustomFileAttachmentNameWidget(QWidget *parent = nullptr);
    ~SnippetCustomFileAttachmentNameWidget();
    Q_REQUIRED_RESULT QString result() const;
    void setText(const QString &str);
private:
    void insertVariable(MessageComposer::ConvertSnippetVariablesUtil::VariableType variable);
    QLineEdit *mLineEdit = nullptr;
    MessageComposer::ConvertSnippetVariableMenu *mConvertMenu = nullptr;
};
}

#endif // SNIPPETCUSTOMFILEATTACHMENTNAMEWIDGET_H
