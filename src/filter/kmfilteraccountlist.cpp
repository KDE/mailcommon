/*
  SPDX-FileCopyrightText: 2014-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kmfilteraccountlist.h"
#include "mailfilter.h"
#include "util/mailutil.h"

#include <Akonadi/AgentInstance>
#include <Akonadi/AgentType>

#include <KLocalizedString>

#include <QHeaderView>

using namespace MailCommon;

KMFilterAccountList::KMFilterAccountList(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(2);
    const QStringList headerNames{i18n("Account Name"), i18n("Type")};
    setHeaderItem(new QTreeWidgetItem(headerNames));
    setAllColumnsShowFocus(true);
    setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
    setSortingEnabled(false);
    setRootIsDecorated(false);
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);
    header()->setSectionsMovable(false);
}

KMFilterAccountList::~KMFilterAccountList() = default;

void KMFilterAccountList::reloadAndSelectAccountsFrom(const MailCommon::MailFilter *filter)
{
    const auto decideSelected = [filter](const QString& accountName) {
        return filter && filter->applyOnAccount(accountName);
    };
    reloadAndSelectAccountsInternal(decideSelected);
}

void KMFilterAccountList::reloadAndSelectAccounts(const QStringList &selectedAccounts)
{
    const auto decideSelected = [&selectedAccounts](const QString& accountName) {
        return selectedAccounts.contains(accountName);
    };
    reloadAndSelectAccountsInternal(decideSelected);
}

void KMFilterAccountList::reloadAndSelectAccountsInternal(const std::function<bool(const QString &)> &decideSelected)
{
    clear();

    QTreeWidgetItem *prev = nullptr;
    // Block the signals here, otherwise we end up calling
    // slotApplicableAccountsChanged(), which will read the incomplete item
    // state and write that back to the filter
    blockSignals(true);
    const Akonadi::AgentInstance::List lst = MailCommon::Util::agentInstances();
    const int nbAccount = lst.count();
    for (int i = 0; i < nbAccount; ++i) {
        const Akonadi::AgentInstance agent = lst.at(i);
        auto listItem = new QTreeWidgetItem(this, prev);
        listItem->setText(0, agent.name());
        listItem->setText(1, agent.type().name());
        listItem->setText(2, agent.identifier());
        listItem->setCheckState(0, decideSelected(agent.identifier()) ? Qt::Checked : Qt::Unchecked);
        prev = listItem;
    }
    blockSignals(false);

    // make sure our hidden column is really hidden (Qt tends to re-show it)
    hideColumn(2);
    resizeColumnToContents(0);
    resizeColumnToContents(1);

    QTreeWidgetItem *top = topLevelItem(0);
    if (top) {
        setCurrentItem(top);
    }
}

QStringList KMFilterAccountList::selectedAccounts() const
{
    QStringList accounts;

    const int count = topLevelItemCount();
    for (int i = 0; i < count; ++i) {
        QTreeWidgetItem *item = topLevelItem(i);
        if (item->checkState(0) == Qt::Checked) {
            accounts.append(item->text(2));
        }
    }
    return accounts;
}

void KMFilterAccountList::setSelectedAccountsOnFilter(MailCommon::MailFilter *filter) const
{
    const int count = topLevelItemCount();
    for (int i = 0; i < count; ++i) {
        const QTreeWidgetItem *const item = topLevelItem(i);
        const QString id = item->text(2);
        filter->setApplyOnAccount(id, item->checkState(0) == Qt::Checked);
    }
}

#include "moc_kmfilteraccountlist.cpp"
