/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingtransportdialog.h"

#include <KLocalizedString>
#include <KSharedConfig>

#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <MailTransport/TransportComboBox>

using namespace MailCommon;

FilterActionMissingTransportDialog::FilterActionMissingTransportDialog(const QString &filtername, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Transport"));
    auto mainLayout = new QVBoxLayout(this);

    auto label = new QLabel(this);
    label->setObjectName(QStringLiteral("label"));
    label->setText(
        i18n("Filter transport is missing. "
             "Please select a transport to use with filter \"%1\"",
             filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mComboBoxTransport = new MailTransport::TransportComboBox(this);
    mComboBoxTransport->setObjectName(QStringLiteral("comboboxtransport"));
    mainLayout->addWidget(mComboBoxTransport);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingTransportDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingTransportDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

FilterActionMissingTransportDialog::~FilterActionMissingTransportDialog()
{
    writeConfig();
}

void FilterActionMissingTransportDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "FilterActionMissingTransportDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingTransportDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "FilterActionMissingTransportDialog");
    group.writeEntry("Size", size());
}

int FilterActionMissingTransportDialog::selectedTransport() const
{
    return mComboBoxTransport->currentTransportId();
}
