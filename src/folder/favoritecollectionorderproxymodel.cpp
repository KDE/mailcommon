/*

  SPDX-FileCopyrightText: 2017 David Faure <faure@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "favoritecollectionorderproxymodel.h"
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
