/*

  Copyright (c) 2009-2019 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAILCOMMON_ENTITYCOLLECTIONORDERPROXYMODEL_H
#define MAILCOMMON_ENTITYCOLLECTIONORDERPROXYMODEL_H

#include <EntityOrderProxyModel>
#include "mailcommon_export.h"

namespace MailCommon {
/**
 * @brief The EntityCollectionOrderProxyModel class implements ordering of mail collections.
 * It supports two modes: manual sorting and automatic sorting.
 *
 * The manual sorting (which has to be activated explicitly by the user) allows the user to
 * reorder the collections (both toplevel resources and folders within the resource) by drag-n-drop,
 * and is implemented by the base class EntityOrderProxyModel.
 *
 * The automatic sorting is implemented by this class itself, and consists of assigning ranks
 * to various special folders (outbox, drafts, sent etc.) and then having the other folders sorted
 * by name (or another column), i.e. the default behaviour from QSortFilterProxyModel.
 * In that mode, the order of the toplevel folders can be controlled with setTopLevelOrder().
 */
class MAILCOMMON_EXPORT EntityCollectionOrderProxyModel : public Akonadi::EntityOrderProxyModel
{
    Q_OBJECT
public:
    explicit EntityCollectionOrderProxyModel(QObject *parent = nullptr);

    ~EntityCollectionOrderProxyModel() override;

    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

    void setManualSortingActive(bool active);
    Q_REQUIRED_RESULT bool isManualSortingActive() const;

    void clearRanks();
    void setTopLevelOrder(const QStringList &list);

public Q_SLOTS:
    void slotSpecialCollectionsChanged();

private:
    class EntityCollectionOrderProxyModelPrivate;
    EntityCollectionOrderProxyModelPrivate *const d;
};
}

#endif
