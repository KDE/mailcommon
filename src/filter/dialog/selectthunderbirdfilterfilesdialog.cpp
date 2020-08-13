/*
  SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "selectthunderbirdfilterfilesdialog.h"
#include "selectthunderbirdfilterfileswidget.h"
#include <KSharedConfig>
#include <KLocalizedString>

#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace MailCommon;

SelectThunderbirdFilterFilesDialog::SelectThunderbirdFilterFilesDialog(const QString &defaultSettingPath, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Select thunderbird filter files"));
    setModal(true);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mSelectFilterFilesWidget = new MailCommon::SelectThunderbirdFilterFilesWidget(defaultSettingPath, this);
    mainLayout->addWidget(mSelectFilterFilesWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
    KConfigGroup group(KSharedConfig::openConfig(), "SelectThunderbirdFilterFilesDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void SelectThunderbirdFilterFilesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SelectThunderbirdFilterFilesDialog");
    group.writeEntry("Size", size());
}
