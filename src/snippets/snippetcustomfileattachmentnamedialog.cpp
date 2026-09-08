/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

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

SnippetCustomFileAttachmentNameDialog::~SnippetCustomFileAttachmentNameDialog()
{
    writeConfig();
}

void SnippetCustomFileAttachmentNameDialog::readConfig()
{
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(mySnippetCustomFileAttachmentNameDialogGroupName), QSize(500, 150));
#else
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 150));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySnippetCustomFileAttachmentNameDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
#endif
}

void SnippetCustomFileAttachmentNameDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySnippetCustomFileAttachmentNameDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
#endif
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
