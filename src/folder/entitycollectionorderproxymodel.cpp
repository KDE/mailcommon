/*

  SPDX-FileCopyrightText: 2010-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "entitycollectionorderproxymodel.h"
#include "kernel/mailkernel.h"
#include "mailcommon_debug.h"
#include "util/mailutil.h"
#include <AgentManager>
#include <Akonadi/KMime/SpecialMailCollections>
#include <Collection>
#include <EntityTreeModel>

namespace MailCommon
{
class Q_DECL_HIDDEN EntityCollectionOrderProxyModel::EntityCollectionOrderProxyModelPrivate
{
public:
    EntityCollectionOrderProxyModelPrivate()
    {
    }

    int collectionRank(const Akonadi::Collection &collection)
    {
        const Akonadi::Collection::Id id = collection.id();
        const int cachedRank = collectionRanks.value(id, -1);
        if (cachedRank != -1) {
            return cachedRank;
        }

        int rank = 100;
        if (Kernel::folderIsInbox(collection)) {
            rank = 1;
        } else if (Kernel::self()->folderIsDraftOrOutbox(collection)) {
            if (Kernel::self()->folderIsDrafts(collection)) {
                rank = 5;
            } else {
                rank = 2;
            }
        } else if (Kernel::self()->folderIsSentMailFolder(collection)) {
            rank = 3;
        } else if (Kernel::self()->folderIsTrash(collection)) {
            rank = 4;
        } else if (Kernel::self()->folderIsTemplates(collection)) {
            rank = 6;
        } else if (MailCommon::Util::isVirtualCollection(collection)) {
            rank = 200;
        } else if (collection.parentCollection() == Akonadi::Collection::root() && MailCommon::Util::isUnifiedMailboxesAgent(collection)) {
            // special treatment for Unified Mailboxes: they are *always* on top
            rank = 0;
        } else if (!topLevelOrder.isEmpty()) {
            if (collection.parentCollection() == Akonadi::Collection::root()) {
                const QString resource = collection.resource();
                if (resource.isEmpty()) {
                    qCDebug(MAILCOMMON_LOG) << " collection has not resource: " << collection;
                    // Don't save in collectionranks because we don't have resource name => pb.
                    return rank;
                }
                const int order = topLevelOrder.indexOf(resource);
                if (order != -1) {
                    rank = order + 1; /* top-level rank "0" belongs to Unified Mailboxes */
                }
            }
        }
        collectionRanks.insert(id, rank);
        return rank;
    }

    QMap<Akonadi::Collection::Id, int> collectionRanks;
    QStringList topLevelOrder;
    bool manualSortingActive = false;
};

EntityCollectionOrderProxyModel::EntityCollectionOrderProxyModel(QObject *parent)
    : EntityOrderProxyModel(parent)
    , d(new EntityCollectionOrderProxyModelPrivate())
{
    setSortCaseSensitivity(Qt::CaseInsensitive);
    connect(Akonadi::SpecialMailCollections::self(),
            &Akonadi::SpecialMailCollections::defaultCollectionsChanged,
            this,
            &EntityCollectionOrderProxyModel::slotSpecialCollectionsChanged);
    connect(Akonadi::SpecialMailCollections::self(),
            &Akonadi::SpecialMailCollections::collectionsChanged,
            this,
            &EntityCollectionOrderProxyModel::slotSpecialCollectionsChanged);
}

EntityCollectionOrderProxyModel::~EntityCollectionOrderProxyModel()
{
    if (d->manualSortingActive) {
        saveOrder();
    }
    delete d;
}

void EntityCollectionOrderProxyModel::slotSpecialCollectionsChanged()
{
    if (!d->manualSortingActive) {
        d->collectionRanks.clear();
        invalidate();
    }
}

void EntityCollectionOrderProxyModel::setTopLevelOrder(const QStringList &list)
{
    d->topLevelOrder = list;
    clearRanks();
}

void EntityCollectionOrderProxyModel::clearRanks()
{
    d->collectionRanks.clear();
    invalidate();
}

bool EntityCollectionOrderProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const auto leftData = left.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
    const auto rightData = right.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
    if (!d->manualSortingActive) {
        const int rankLeft = d->collectionRank(leftData);
        const int rankRight = d->collectionRank(rightData);

        if (rankLeft < rankRight) {
            return true;
        } else if (rankLeft > rankRight) {
            return false;
        }

        return QSortFilterProxyModel::lessThan(left, right);
    }

    if (MailCommon::Util::isUnifiedMailboxesAgent(leftData)) {
        return true;
    } else {
        return EntityOrderProxyModel::lessThan(left, right);
    }
}

void EntityCollectionOrderProxyModel::setManualSortingActive(bool active)
{
    if (d->manualSortingActive == active) {
        return;
    }

    d->manualSortingActive = active;
    d->collectionRanks.clear();
    invalidate();
}

bool EntityCollectionOrderProxyModel::isManualSortingActive() const
{
    return d->manualSortingActive;
}
}
