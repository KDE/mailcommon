/*

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_ENTITYCOLLECTIONORDERPROXYMODEL_H
#define MAILCOMMON_ENTITYCOLLECTIONORDERPROXYMODEL_H

#include "mailcommon_export.h"
#include <EntityOrderProxyModel>

namespace MailCommon
{
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
