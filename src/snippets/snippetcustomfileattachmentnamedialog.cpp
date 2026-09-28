/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "snippetcustomfileattachmentnamedialog.h"
#include "snippetcustomfileattachmentnamewidget.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
static const char mySnippetCustomFileAttachmentNameDialogGroupName[] = "SnippetCustomFileAttachmentNameDialog";
}
using namespace MailCommon;
SnippetCustomFileAttachmentNameDialog::SnippetCustomFileAttachmentNameDialog(QWidget *parent)
    : QDialog(parent)
    , mCustomWidget(new SnippetCustomFileAttachmentNameWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Customize File Name Attachment"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QLatin1StringView("mainLayout"));

    mCustomWidget->setObjectName(QLatin1StringView("mCustomWidget"));
    mainLayout->addWidget(mCustomWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QLatin1StringView("button"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SnippetCustomFileAttachmentNameDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SnippetCustomFileAttachmentNameDialog::reject);

    mainLayout->addWidget(buttonBox);

    readConfig();
}

SnippetCustomFileAttachmentNameDialog::~SnippetCustomFileAttachmentNameDialog() = default;

void SnippetCustomFileAttachmentNameDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(mySnippetCustomFileAttachmentNameDialogGroupName), QSize(500, 150));
}

QString SnippetCustomFileAttachmentNameDialog::result() const
{
    return mCustomWidget->result();
}

void SnippetCustomFileAttachmentNameDialog::setText(const QString &str)
{
    mCustomWidget->setText(str);
}

#include "moc_snippetcustomfileattachmentnamedialog.cpp"
