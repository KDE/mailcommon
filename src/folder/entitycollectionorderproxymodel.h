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

/*!
 * \qmltype EntityCollectionOrderProxyModel
 * \inqmlmodule org.kde.mailcommon
 * \nativetype MailCommon::EntityCollectionOrderProxyModel
 * \brief A model that orders mail collections and folders.
 */
/*!
 * \class MailCommon::EntityCollectionOrderProxyModel
 * \inmodule MailCommon
 * \inheaderfile MailCommon/EntityCollectionOrderProxyModel
 *
 * \brief The EntityCollectionOrderProxyModel class implements ordering of mail collections.
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
    /*!
     * Constructs a collection order proxy model with the given \a parent.
     */
    explicit EntityCollectionOrderProxyModel(QObject *parent = nullptr);

    /*!
     * Destroys the model. The manual order is saved if manual sorting is active.
     */
    ~EntityCollectionOrderProxyModel() override;

    /*!
     * Returns \c true if the collection at \a left must be sorted before the collection at \a right.
     */
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

    /*!
     * Enables manual sorting (by drag-n-drop) if \a active is \c true, otherwise automatic sorting is used.
     */
    void setManualSortingActive(bool active);
    /*!
     * Returns \c true if manual sorting is active.
     */
    [[nodiscard]] bool isManualSortingActive() const;

    /*!
     * Clears the cached ranks of the collections and re-sorts the model.
     */
    void clearRanks();
    /*!
     * Sets the order of the toplevel collections to \a list, a list of resource identifiers.
     */
    void setTopLevelOrder(const QStringList &list);

    /*!
     * Sets the \a matcher used to filter the folders.
     */
    void setFolderMatcher(const HierarchicalFolderMatcher &matcher);

public Q_SLOTS:
    /*!
     * Updates the ranks when the special collections change.
     */
    void slotSpecialCollectionsChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    class EntityCollectionOrderProxyModelPrivate;
    std::unique_ptr<EntityCollectionOrderProxyModelPrivate> const d;
};
}
