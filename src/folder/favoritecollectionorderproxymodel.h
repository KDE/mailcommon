/*

  Copyright (c) 2017 David Faure <faure@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAILCOMMON_FAVORITECOLLECTIONORDERPROXYMODEL_H
#define MAILCOMMON_FAVORITECOLLECTIONORDERPROXYMODEL_H

#include <EntityOrderProxyModel>
#include "mailcommon_export.h"

namespace MailCommon {
/**
 * @brief The FavoriteCollectionOrderProxyModel class implements ordering of favorite collections.
 */
class MAILCOMMON_EXPORT FavoriteCollectionOrderProxyModel : public Akonadi::EntityOrderProxyModel
{
    Q_OBJECT
public:
    explicit FavoriteCollectionOrderProxyModel(QObject *parent = nullptr);
    virtual ~FavoriteCollectionOrderProxyModel();

protected:
    Akonadi::Collection parentCollection(const QModelIndex &index) const override;

private:
    class FavoriteCollectionOrderProxyModelPrivate;
    FavoriteCollectionOrderProxyModelPrivate *const d;
};
}

#endif
