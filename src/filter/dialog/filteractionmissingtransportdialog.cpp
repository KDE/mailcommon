/*
   SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingtransportdialog.h"

#include <KLocalizedString>
#include <KSharedConfig>

#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

#include <MailTransport/TransportComboBox>

using namespace MailCommon;
namespace
{
static const char myFilterActionMissingTransportDialogGroupName[] = "FilterActionMissingTransportDialog";
}
FilterActionMissingTransportDialog::FilterActionMissingTransportDialog(const QString &filtername, QWidget *parent)
    : QDialog(parent)
    , mComboBoxTransport(new MailTransport::TransportComboBox(this))
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Transport"));
    auto mainLayout = new QVBoxLayout(this);

    auto label = new QLabel(this);
    label->setObjectName(QLatin1StringView("label"));
    label->setText(
        i18n("Filter transport is missing. "
             "Please select a transport to use with filter \"%1\"",
             filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mComboBoxTransport->setObjectName(QLatin1StringView("comboboxtransport"));
    mainLayout->addWidget(mComboBoxTransport);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QLatin1StringView("buttonbox"));
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myFilterActionMissingTransportDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void FilterActionMissingTransportDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myFilterActionMissingTransportDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

int FilterActionMissingTransportDialog::selectedTransport() const
{
    return mComboBoxTransport->currentTransportId();
}

#include "moc_filteractionmissingtransportdialog.cpp"
