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

#include "snippetcustomfileattachmentnamedialog.h"
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include "snippetcustomfileattachmentnamewidget.h"

using namespace MailCommon;
SnippetCustomFileAttachmentNameDialog::SnippetCustomFileAttachmentNameDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCustomWidget = new SnippetCustomFileAttachmentNameWidget(this);
    mCustomWidget->setObjectName(QStringLiteral("mCustomWidget"));
    mainLayout->addWidget(mCustomWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SnippetCustomFileAttachmentNameDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SnippetCustomFileAttachmentNameDialog::reject);

    mainLayout->addWidget(buttonBox);
}

SnippetCustomFileAttachmentNameDialog::~SnippetCustomFileAttachmentNameDialog()
{

}

QString SnippetCustomFileAttachmentNameDialog::result() const
{
    return mCustomWidget->result();
}

void SnippetCustomFileAttachmentNameDialog::setText(const QString &str)
{
    mCustomWidget->setText(str);
}
