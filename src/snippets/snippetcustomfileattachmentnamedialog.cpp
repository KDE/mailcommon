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

#include "snippetcustomfileattachmentnamedialog.h"
#include "snippetcustomfileattachmentnamewidget.h"

#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <KSharedConfig>
namespace {
static const char myConfigGroupName[] = "SnippetCustomFileAttachmentNameDialog";
}
using namespace MailCommon;
SnippetCustomFileAttachmentNameDialog::SnippetCustomFileAttachmentNameDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Customize File Name Attachment"));
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

    readConfig();
}

SnippetCustomFileAttachmentNameDialog::~SnippetCustomFileAttachmentNameDialog()
{
    writeConfig();
}

void SnippetCustomFileAttachmentNameDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);

    const QSize size = group.readEntry("Size", QSize(500, 150));
    if (size.isValid()) {
        resize(size);
    }
}

void SnippetCustomFileAttachmentNameDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

QString SnippetCustomFileAttachmentNameDialog::result() const
{
    return mCustomWidget->result();
}

void SnippetCustomFileAttachmentNameDialog::setText(const QString &str)
{
    mCustomWidget->setText(str);
}
