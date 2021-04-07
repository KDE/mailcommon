/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "snippetselectattachmentdialog.h"
#include "snippetselectattachmentwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
namespace
{
static const char mySnippetSelectAttachmentDialogGroupName[] = "SnippetSelectAttachmentDialog";
}
using namespace MailCommon;
SnippetSelectAttachmentDialog::SnippetSelectAttachmentDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Select Attachments"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mAttachmentWidget = new SnippetSelectAttachmentWidget(this);
    mAttachmentWidget->setObjectName(QStringLiteral("attachmentwidget"));
    mainLayout->addWidget(mAttachmentWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
    KConfigGroup group(KSharedConfig::openStateConfig(), mySnippetSelectAttachmentDialogGroupName);
    group.writeEntry("Size", size());
}

void SnippetSelectAttachmentDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), mySnippetSelectAttachmentDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 350));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}
