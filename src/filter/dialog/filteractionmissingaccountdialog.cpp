/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingaccountdialog.h"
#include "filter/kmfilteraccountlist.h"
#include "util/mailutil.h"

#include <KLocalizedString>
#include <KSharedConfig>

#include <KConfigGroup>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

using namespace MailCommon;
namespace
{
static const char myFilterActionMissingAccountDialogConfigGroupName[] = "FilterActionMissingAccountDialog";
}

FilterActionMissingAccountDialog::FilterActionMissingAccountDialog(const QStringList &accounts, const QString &filtername, QWidget *parent)
    : QDialog(parent)
    , mAccountsSelector(new MailCommon::KMFilterAccountList(this))
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Account"));
    auto mainLayout = new QVBoxLayout(this);

    auto label = new QLabel(this);
    label->setObjectName(QLatin1StringView("label"));
    label->setText(
        i18n("Filter account is missing. "
             "Please select account to use with filter \"%1\"",
             filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mAccountsSelector->setObjectName(QLatin1StringView("accountlist"));
    mAccountsSelector->reloadAndSelectAccounts(accounts);
    mainLayout->addWidget(mAccountsSelector);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QLatin1StringView("buttonbox"));
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 300));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myFilterActionMissingAccountDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void FilterActionMissingAccountDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myFilterActionMissingAccountDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

QStringList FilterActionMissingAccountDialog::selectedAccounts() const
{
    return mAccountsSelector->selectedAccounts();
}

bool FilterActionMissingAccountDialog::allAccountsExist(const QStringList &lst)
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

#include "moc_filteractionmissingaccountdialog.cpp"
