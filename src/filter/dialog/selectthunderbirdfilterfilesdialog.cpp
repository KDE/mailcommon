/*
  Copyright (c) 2012-2019 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
