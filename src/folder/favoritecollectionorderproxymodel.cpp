/*

  SPDX-FileCopyrightText: 2017 David Faure <faure@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "favoritecollectionorderproxymodel.h"
#include <Akonadi/AccountActivitiesAbstract>
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
        disconnect(mAccountActivities, &Akonadi::AccountActivitiesAbstract::activitiesChanged, this, &FavoriteCollectionOrderProxyModel::invalidateFilter);
    }
    mAccountActivities = accountActivities;
    connect(mAccountActivities, &Akonadi::AccountActivitiesAbstract::activitiesChanged, this, &FavoriteCollectionOrderProxyModel::invalidateFilter);
    invalidateFilter();
}

bool FavoriteCollectionOrderProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    // TODO
    return Akonadi::EntityOrderProxyModel::filterAcceptsColumn(sourceRow, sourceParent);
}

#include "moc_favoritecollectionorderproxymodel.cpp"
