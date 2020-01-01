/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "snippetselectattachmentdialog.h"
#include "snippetselectattachmentwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
namespace {
static const char myConfigGroupName[] = "SnippetSelectAttachmentDialog";
}
using namespace MailCommon;
SnippetSelectAttachmentDialog::SnippetSelectAttachmentDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Select Attachments"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mAttachmentWidget = new SnippetSelectAttachmentWidget(this);
    mAttachmentWidget->setObjectName(QStringLiteral("attachmentwidget"));
    mainLayout->addWidget(mAttachmentWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SnippetSelectAttachmentDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SnippetSelectAttachmentDialog::reject);

    mainLayout->addWidget(buttonBox);

    readConfig();
}

SnippetSelectAttachmentDialog::~SnippetSelectAttachmentDialog()
{
    writeConfig();
}

void SnippetSelectAttachmentDialog::setAttachments(const QStringList &lst)
{
    mAttachmentWidget->setAttachments(lst);
}

QStringList SnippetSelectAttachmentDialog::attachments() const
{
    return mAttachmentWidget->attachments();
}

void SnippetSelectAttachmentDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void SnippetSelectAttachmentDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 350));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}
