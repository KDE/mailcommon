/*
   Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
