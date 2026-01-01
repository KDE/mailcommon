/*

  SPDX-FileCopyrightText: 2009-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/EntityOrderProxyModel>

namespace MailCommon
{
class HierarchicalFolderMatcher;

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

    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

    void setManualSortingActive(bool active);
    [[nodiscard]] bool isManualSortingActive() const;

    void clearRanks();
    void setTopLevelOrder(const QStringList &list);

    void setFolderMatcher(const HierarchicalFolderMatcher &matcher);

public Q_SLOTS:
    void slotSpecialCollectionsChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    class EntityCollectionOrderProxyModelPrivate;
    std::unique_ptr<EntityCollectionOrderProxyModelPrivate> const d;
};
}
