/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "snippetselectattachmentdialog.h"
#include "snippetselectattachmentwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
static const char mySnippetSelectAttachmentDialogGroupName[] = "SnippetSelectAttachmentDialog";
}
using namespace MailCommon;
SnippetSelectAttachmentDialog::SnippetSelectAttachmentDialog(QWidget *parent)
    : QDialog(parent)
    , mAttachmentWidget(new SnippetSelectAttachmentWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Select Attachments"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QLatin1StringView("mainLayout"));

    mAttachmentWidget->setObjectName(QLatin1StringView("attachmentwidget"));
    mainLayout->addWidget(mAttachmentWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QLatin1StringView("button"));
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

void SnippetSelectAttachmentDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(300, 350));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySnippetSelectAttachmentDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SnippetSelectAttachmentDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySnippetSelectAttachmentDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

#include "moc_snippetselectattachmentdialog.cpp"
