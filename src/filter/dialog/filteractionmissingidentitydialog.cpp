/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingidentitydialog.h"
#include "kernel/mailkernel.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>

#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <KIdentityManagement/IdentityCombo>

using namespace MailCommon;

FilterActionMissingIdentityDialog::FilterActionMissingIdentityDialog(const QString &filtername, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Identity"));
    auto mainLayout = new QVBoxLayout(this);

    auto label = new QLabel(this);
    label->setObjectName(QStringLiteral("label"));
    label->setText(
        i18n("Filter identity is missing. "
             "Please select an identity to use with filter \"%1\"",
             filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mComboBoxIdentity = new KIdentityManagement::IdentityCombo(KernelIf->identityManager(), this);
    mComboBoxIdentity->setObjectName(QStringLiteral("comboboxidentity"));
    mainLayout->addWidget(mComboBoxIdentity);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
    KConfigGroup group(KSharedConfig::openStateConfig(), "FilterActionMissingMissingIdentity");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingIdentityDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "FilterActionMissingMissingIdentity");
    group.writeEntry("Size", size());
}

int FilterActionMissingIdentityDialog::selectedIdentity() const
{
    return mComboBoxIdentity->currentIdentity();
}
