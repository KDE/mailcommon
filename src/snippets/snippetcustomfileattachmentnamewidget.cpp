/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "snippetcustomfileattachmentnamewidget.h"
#include <MessageComposer/ConvertSnippetVariableMenu>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include <KLocalizedString>
#include <QLabel>

using namespace MailCommon;
SnippetCustomFileAttachmentNameWidget::SnippetCustomFileAttachmentNameWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto lineEditLayout = new QHBoxLayout;
    lineEditLayout->setObjectName(QStringLiteral("lineEditLayout"));
    lineEditLayout->setContentsMargins(0, 0, 0, 0);

    auto label = new QLabel(i18n("Filename:"), this);
    label->setObjectName(QStringLiteral("label"));
    lineEditLayout->addWidget(label);

    mLineEdit = new QLineEdit(this);
    mLineEdit->setObjectName(QStringLiteral("mLineEdit"));
    lineEditLayout->addWidget(mLineEdit);

    mainLayout->addLayout(lineEditLayout);

    auto convertMenuLayout = new QHBoxLayout;
    convertMenuLayout->setObjectName(QStringLiteral("convertMenuLayout"));
    convertMenuLayout->setContentsMargins(0, 0, 0, 0);
    convertMenuLayout->addStretch(1);

    mConvertMenu = new MessageComposer::ConvertSnippetVariableMenu(true, mLineEdit, this);
    mConvertMenu->setObjectName(QStringLiteral("mConvertMenu"));
    connect(mConvertMenu, &MessageComposer::ConvertSnippetVariableMenu::insertVariable, this, &SnippetCustomFileAttachmentNameWidget::insertVariable);

    auto selectVariable = new QPushButton(i18n("Insert Variables"), this);
    selectVariable->setObjectName(QStringLiteral("selectVariable"));
    selectVariable->setMenu(mConvertMenu->menu());
    convertMenuLayout->addWidget(selectVariable);
    mainLayout->addLayout(convertMenuLayout);
    mainLayout->addStretch(1);
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
