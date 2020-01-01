/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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

#include "filteractionmissingtransportdialog.h"

#include <KSharedConfig>
#include <KLocalizedString>

#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include <MailTransport/TransportComboBox>

using namespace MailCommon;

FilterActionMissingTransportDialog::FilterActionMissingTransportDialog(const QString &filtername, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Transport"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel(this);
    label->setObjectName(QStringLiteral("label"));
    label->setText(i18n("Filter transport is missing. "
                        "Please select a transport to use with filter \"%1\"",
                        filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mComboBoxTransport = new MailTransport::TransportComboBox(this);
    mComboBoxTransport->setObjectName(QStringLiteral("comboboxtransport"));
    mainLayout->addWidget(mComboBoxTransport);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingTransportDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingTransportDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingTransportDialog");
    group.writeEntry("Size", size());
}

int FilterActionMissingTransportDialog::selectedTransport() const
{
    return mComboBoxTransport->currentTransportId();
}
