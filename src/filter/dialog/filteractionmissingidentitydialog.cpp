/*
   SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingidentitydialog.h"
#include "kernel/mailkernel.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>

#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

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

FilterActionMissingIdentityDialog::~FilterActionMissingIdentityDialog()
{
    writeConfig();
}

void FilterActionMissingIdentityDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myFilterActionMissingIdentityDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void FilterActionMissingIdentityDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myFilterActionMissingIdentityDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

int FilterActionMissingIdentityDialog::selectedIdentity() const
{
    return mComboBoxIdentity->currentIdentity();
}

#include "moc_filteractionmissingidentitydialog.cpp"
