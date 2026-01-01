/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "selectthunderbirdfilterfilesdialog.h"
#include "selectthunderbirdfilterfileswidget.h"
#include <KLocalizedString>
#include <KSharedConfig>

#include <KConfigGroup>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

using namespace MailCommon;
namespace
{
static const char mySelectThunderbirdFilterFilesDialogGroupName[] = "SelectThunderbirdFilterFilesDialog";
}
SelectThunderbirdFilterFilesDialog::SelectThunderbirdFilterFilesDialog(const QString &defaultSettingPath, QWidget *parent)
    : QDialog(parent)
    , mSelectFilterFilesWidget(new MailCommon::SelectThunderbirdFilterFilesWidget(defaultSettingPath, this))
{
    setWindowTitle(i18nc("@title:window", "Select thunderbird filter files"));
    setModal(true);
    auto mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(mSelectFilterFilesWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectThunderbirdFilterFilesDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectThunderbirdFilterFilesDialog::reject);
    connect(mSelectFilterFilesWidget, &MailCommon::SelectThunderbirdFilterFilesWidget::enableOkButton, okButton, &QPushButton::setEnabled);
    okButton->setEnabled(false);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

SelectThunderbirdFilterFilesDialog::~SelectThunderbirdFilterFilesDialog()
{
    writeConfig();
}

QStringList SelectThunderbirdFilterFilesDialog::selectedFiles() const
{
    return mSelectFilterFilesWidget->selectedFiles();
}

void SelectThunderbirdFilterFilesDialog::setStartDir(const QUrl &url)
{
    mSelectFilterFilesWidget->setStartDir(url);
}

void SelectThunderbirdFilterFilesDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 300));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySelectThunderbirdFilterFilesDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SelectThunderbirdFilterFilesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySelectThunderbirdFilterFilesDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

#include "moc_selectthunderbirdfilterfilesdialog.cpp"
