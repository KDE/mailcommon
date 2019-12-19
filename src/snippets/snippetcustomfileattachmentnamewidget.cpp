/*
   Copyright (C) 2019 Laurent Montel <montel@kde.org>

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

#include "snippetcustomfileattachmentnamewidget.h"
#include <MessageComposer/ConvertSnippetVariableMenu>
#include <QHBoxLayout>
#include <KLocalizedString>
#include <QLineEdit>
#include <QPushButton>
using namespace MailCommon;
SnippetCustomFileAttachmentNameWidget::SnippetCustomFileAttachmentNameWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mLineEdit = new QLineEdit(this);
    mLineEdit->setObjectName(QStringLiteral("mLineEdit"));
    mainLayout->addWidget(mLineEdit);

    mConvertMenu = new MessageComposer::ConvertSnippetVariableMenu(true, mLineEdit, this);
    mConvertMenu->setObjectName(QStringLiteral("mConvertMenu"));
    connect(mConvertMenu, &MessageComposer::ConvertSnippetVariableMenu::insertVariable, this, &SnippetCustomFileAttachmentNameWidget::insertVariable);

    QPushButton *selectVariable = new QPushButton(i18n("Variables"), this);
    selectVariable->setObjectName(QStringLiteral("selectVariable"));
    selectVariable->setMenu(mConvertMenu->menu());
    mainLayout->addWidget(selectVariable);
}

SnippetCustomFileAttachmentNameWidget::~SnippetCustomFileAttachmentNameWidget()
{
}

void SnippetCustomFileAttachmentNameWidget::insertVariable(MessageComposer::ConvertSnippetVariablesUtil::VariableType variable)
{
    mLineEdit->insert(MessageComposer::ConvertSnippetVariablesUtil::snippetVariableFromEnum(variable));
}

QString SnippetCustomFileAttachmentNameWidget::result() const
{
    return mLineEdit->text();
}

void SnippetCustomFileAttachmentNameWidget::setText(const QString &str)
{
    mLineEdit->setText(str);
}
