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


#include "filteractionmissingaccountdialog.h"
#include "filter/kmfilteraccountlist.h"
#include "util/mailutil.h"

#include <KLocalizedString>
#include <KSharedConfig>

#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

using namespace MailCommon;

FilterActionMissingAccountDialog::FilterActionMissingAccountDialog(const QStringList &lstAccount,
        const QString &filtername,
        QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18n("Select Account"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QWidget *mainWidget = new QWidget(this);
    mainLayout->addWidget(mainWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingAccountDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingAccountDialog::reject);
    mainLayout->addWidget(buttonBox);
    okButton->setDefault(true);
    QVBoxLayout *lay = new QVBoxLayout(mainWidget);
    QLabel *label = new QLabel(this);
    label->setText(i18n("Filter account is missing. "
                        "Please select account to use with filter \"%1\"",
                        filtername));
    label->setWordWrap(true);
    lay->addWidget(label);
    mAccountList = new MailCommon::KMFilterAccountList(this);
    mAccountList->applyOnAccount(lstAccount);
    lay->addWidget(mAccountList);
    readConfig();
}

FilterActionMissingAccountDialog::~FilterActionMissingAccountDialog()
{
    writeConfig();
}

void FilterActionMissingAccountDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingAccountDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingAccountDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingAccountDialog");
    group.writeEntry("Size", size());
}

QStringList FilterActionMissingAccountDialog::selectedAccount() const
{
    return mAccountList->selectedAccount();
}

bool FilterActionMissingAccountDialog::allAccountExist(const QStringList &lst)
{
    const Akonadi::AgentInstance::List lstAgent = MailCommon::Util::agentInstances();

    const int numberOfAccount(lst.count());
    const int numberOfAgent(lstAgent.count());

    for (int i = 0; i < numberOfAccount; ++i) {
        bool found = false;
        const QString accountName(lst.at(i));
        for (int j = 0; j < numberOfAgent; ++j) {
            if (lstAgent.at(j).identifier() ==  accountName) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}
