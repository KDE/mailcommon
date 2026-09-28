/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingidentitydialog.h"
#include "kernel/mailkernel.h"

#include <KLocalizedString>

#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

#include <KIdentityManagementWidgets/IdentityCombo>

using namespace MailCommon;
namespace
{
static const char myFilterActionMissingIdentityDialogConfigGroupName[] = "FilterActionMissingIdentityDialog";
}
FilterActionMissingIdentityDialog::FilterActionMissingIdentityDialog(const QString &filtername, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Identity"));
    auto mainLayout = new QVBoxLayout(this);

    auto label = new QLabel(this);
    label->setObjectName(QLatin1StringView("label"));
    label->setText(
        i18n("Filter identity is missing. "
             "Please select an identity to use with filter \"%1\"",
             filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mComboBoxIdentity = new KIdentityManagementWidgets::IdentityCombo(KernelIf->identityManager(), this);
    mComboBoxIdentity->setObjectName(QLatin1StringView("comboboxidentity"));
    mainLayout->addWidget(mComboBoxIdentity);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QLatin1StringView("buttonbox"));

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingIdentityDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingIdentityDialog::reject);
    mainLayout->addWidget(buttonBox);
    okButton->setDefault(true);

    readConfig();
}

FilterActionMissingIdentityDialog::~FilterActionMissingIdentityDialog() = default;

void FilterActionMissingIdentityDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myFilterActionMissingIdentityDialogConfigGroupName), QSize(500, 300));
}

int FilterActionMissingIdentityDialog::selectedIdentity() const
{
    return mComboBoxIdentity->currentIdentity();
}

#include "moc_filteractionmissingidentitydialog.cpp"
