/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "snippetcustomfileattachmentnamedialog.h"
#include "snippetcustomfileattachmentnamewidget.h"

#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QWindow>
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
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCustomWidget->setObjectName(QStringLiteral("mCustomWidget"));
    mainLayout->addWidget(mCustomWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 150));
    KConfigGroup group(KSharedConfig::openStateConfig(), mySnippetCustomFileAttachmentNameDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SnippetCustomFileAttachmentNameDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), mySnippetCustomFileAttachmentNameDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

QString SnippetCustomFileAttachmentNameDialog::result() const
{
    return mCustomWidget->result();
}

void SnippetCustomFileAttachmentNameDialog::setText(const QString &str)
{
    mCustomWidget->setText(str);
}
