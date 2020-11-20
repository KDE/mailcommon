/*

  SPDX-FileCopyrightText: 2017 David Faure <faure@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "favoritecollectionorderproxymodel.h"
#include "mailcommon_debug.h"
#include <Collection>
#include <EntityTreeModel>

using namespace MailCommon;

FavoriteCollectionOrderProxyModel::FavoriteCollectionOrderProxyModel(QObject *parent)
    : EntityOrderProxyModel(parent)
{
}

FavoriteCollectionOrderProxyModel::~FavoriteCollectionOrderProxyModel()
{
}

Akonadi::Collection FavoriteCollectionOrderProxyModel::parentCollection(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return {};
}
