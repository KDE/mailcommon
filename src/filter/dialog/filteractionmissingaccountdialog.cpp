/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingaccountdialog.h"
#include "filter/kmfilteraccountlist.h"
#include "util/mailutil.h"

#include <KLocalizedString>
#include <KSharedConfig>

#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace MailCommon;

FilterActionMissingAccountDialog::FilterActionMissingAccountDialog(const QStringList &lstAccount, const QString &filtername, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Account"));
    auto mainLayout = new QVBoxLayout(this);

    auto label = new QLabel(this);
    label->setObjectName(QStringLiteral("label"));
    label->setText(
        i18n("Filter account is missing. "
             "Please select account to use with filter \"%1\"",
             filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mAccountList = new MailCommon::KMFilterAccountList(this);
    mAccountList->setObjectName(QStringLiteral("accountlist"));
    mAccountList->applyOnAccount(lstAccount);
    mainLayout->addWidget(mAccountList);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingAccountDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingAccountDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

FilterActionMissingAccountDialog::~FilterActionMissingAccountDialog()
{
    writeConfig();
}

void FilterActionMissingAccountDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "FilterActionMissingAccountDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingAccountDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "FilterActionMissingAccountDialog");
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
            if (lstAgent.at(j).identifier() == accountName) {
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
