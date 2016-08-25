/*
   Copyright (C) 2016 Laurent Montel <montel@kde.org>

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


#include "filteractionmissingidentitydialog.h"
#include "kernel/mailkernel.h"

#include <KSharedConfig>
#include <KConfigGroup>
#include <KLocalizedString>

#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <KIdentityManagement/IdentityCombo>

using namespace MailCommon;

FilterActionMissingIdentityDialog::FilterActionMissingIdentityDialog(const QString &filtername,
        QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18n("Select Identity"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel(this);
    label->setObjectName(QStringLiteral("label"));
    label->setText(i18n("Filter identity is missing. "
                        "Please select an identity to use with filter \"%1\"",
                        filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mComboBoxIdentity = new KIdentityManagement::IdentityCombo(KernelIf->identityManager(), this);
    mComboBoxIdentity->setObjectName(QStringLiteral("comboboxidentity"));
    mainLayout->addWidget(mComboBoxIdentity);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingIdentityDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingIdentityDialog::reject);
    mainLayout->addWidget(buttonBox);
    okButton->setDefault(true);

    readConfig();
}

FilterActionMissingIdentityDialog::~FilterActionMissingIdentityDialog()
{
    writeConfig();
}

void FilterActionMissingIdentityDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingMissingIdentity");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingIdentityDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingMissingIdentity");
    group.writeEntry("Size", size());
}

int FilterActionMissingIdentityDialog::selectedIdentity() const
{
    return mComboBoxIdentity->currentIdentity();
}

