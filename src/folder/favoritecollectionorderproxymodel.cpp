/*

  SPDX-FileCopyrightText: 2017 David Faure <faure@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "favoritecollectionorderproxymodel.h"
#include <Akonadi/AccountActivitiesAbstract>
#include <Akonadi/AgentManager>
#include <Akonadi/Collection>
#include <Akonadi/EntityTreeModel>
using namespace MailCommon;

FavoriteCollectionOrderProxyModel::FavoriteCollectionOrderProxyModel(QObject *parent)
    : EntityOrderProxyModel(parent)
{
}

FavoriteCollectionOrderProxyModel::~FavoriteCollectionOrderProxyModel() = default;

Akonadi::Collection FavoriteCollectionOrderProxyModel::parentCollection(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return {};
}

void FavoriteCollectionOrderProxyModel::setAccountActivities(Akonadi::AccountActivitiesAbstract *accountActivities)
{
    if (mAccountActivities) {
        disconnect(mAccountActivities, &Akonadi::AccountActivitiesAbstract::activitiesChanged, this, &FavoriteCollectionOrderProxyModel::slotInvalidateFilter);
    }
    mAccountActivities = accountActivities;
    connect(mAccountActivities, &Akonadi::AccountActivitiesAbstract::activitiesChanged, this, &FavoriteCollectionOrderProxyModel::slotInvalidateFilter);
    slotInvalidateFilter();
}

bool FavoriteCollectionOrderProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (mAccountActivities) {
        const QModelIndex modelIndex = sourceModel()->index(sourceRow, 0, sourceParent);

        const auto collection = sourceModel()->data(modelIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        const Akonadi::AgentInstance instance = Akonadi::AgentManager::self()->instance(collection.resource());
        if (instance.activitiesEnabled()) {
            return mAccountActivities->filterAcceptsRow(instance.activities());
        }
    }
    return Akonadi::EntityOrderProxyModel::filterAcceptsColumn(sourceRow, sourceParent);
}

void FavoriteCollectionOrderProxyModel::slotInvalidateFilter()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    beginFilterChange();
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
    invalidateFilter();
#endif
}

#include "moc_favoritecollectionorderproxymodel.cpp"
