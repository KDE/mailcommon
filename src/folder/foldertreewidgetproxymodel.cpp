/*
  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "foldertreewidgetproxymodel.h"
#include "foldersettings.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"

#include <MessageCore/MessageCoreSettings>

#include <AgentInstance>
#include <AgentManager>
#include <EntityTreeModel>
#include <MimeTypeChecker>

#include <KColorScheme>
#include <KLocalizedString>
#include <collectionquotaattribute.h>

#include <QIcon>
#include <QPalette>

namespace MailCommon
{
class FolderTreeWidgetProxyModel::Private
{
public:
    Private(FolderTreeWidgetProxyModel *qq)
        : q(qq)
    {
    }

    bool checkQuotaExcedded(const QModelIndex &index, qreal &percentage)
    {
        if (index.model()->hasChildren(index)) {
            const int rowCount = index.model()->rowCount(index);
            for (int row = 0; row < rowCount; row++) {
                const QModelIndex firstIndex = q->mapToSource(index.model()->index(row, 0, index));

                const auto collectionFirst = q->sourceModel()->data(firstIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
                if (collectionFirst.isValid() && collectionFirst.hasAttribute<Akonadi::CollectionQuotaAttribute>()) {
                    const auto *quota = collectionFirst.attribute<Akonadi::CollectionQuotaAttribute>();

                    if (quota->currentValue() > -1 && quota->maximumValue() > 0) {
                        percentage = (100.0 * quota->currentValue()) / quota->maximumValue();
                        if (percentage >= threshold) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    QSet<QString> includedMimeTypes;
    Akonadi::MimeTypeChecker checker;

    QColor brokenAccountColor;
    qreal threshold = 0.0;
    FolderTreeWidgetProxyModel *const q;
    bool enableCheck = false;
    bool hideVirtualFolder = false;
    bool hideSpecificFolder = false;
    bool hideOutboxFolder = false;
};

FolderTreeWidgetProxyModel::FolderTreeWidgetProxyModel(QObject *parent, FolderTreeWidgetProxyModelOptions option)
    : Akonadi::EntityRightsFilterModel(parent)
    , d(new Private(this))
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);

    if (option & HideVirtualFolder) {
        d->hideVirtualFolder = true;
    }
    if (option & HideSpecificFolder) {
        d->hideSpecificFolder = true;
    }
    if (option & HideOutboxFolder) {
        d->hideOutboxFolder = true;
    }
    readConfig();
}

FolderTreeWidgetProxyModel::~FolderTreeWidgetProxyModel()
{
    delete d;
}

void FolderTreeWidgetProxyModel::setWarningThreshold(qreal threshold)
{
    d->threshold = threshold;
}

void FolderTreeWidgetProxyModel::readConfig()
{
    invalidate();
}

Qt::ItemFlags FolderTreeWidgetProxyModel::flags(const QModelIndex &index) const
{
    if (d->enableCheck) {
        const QModelIndex sourceIndex = mapToSource(index);
        const QModelIndex rowIndex = sourceIndex.sibling(sourceIndex.row(), 0);
        const auto collection = sourceModel()->data(rowIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        if (!MailCommon::Util::isVirtualCollection(collection)) {
            const Akonadi::AgentInstance instance = Akonadi::AgentManager::self()->instance(collection.resource());
            if (instance.status() == Akonadi::AgentInstance::Broken) {
                return QSortFilterProxyModel::flags(sourceIndex) & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            }
        }
        return Akonadi::EntityRightsFilterModel::flags(index);
    }
    return QSortFilterProxyModel::flags(index);
}

void FolderTreeWidgetProxyModel::setEnabledCheck(bool enable)
{
    if (d->enableCheck == enable) {
        return;
    }

    d->enableCheck = enable;
    if (enable) {
        setAccessRights(Akonadi::Collection::CanCreateItem | Akonadi::Collection::CanCreateCollection);
    }
}

bool FolderTreeWidgetProxyModel::enabledCheck() const
{
    return d->enableCheck;
}

void FolderTreeWidgetProxyModel::setHideVirtualFolder(bool exclude)
{
    if (d->hideVirtualFolder != exclude) {
        d->hideVirtualFolder = exclude;
        invalidate();
    }
}

bool FolderTreeWidgetProxyModel::hideVirtualFolder() const
{
    return d->hideVirtualFolder;
}

void FolderTreeWidgetProxyModel::setHideSpecificFolder(bool hide)
{
    if (d->hideSpecificFolder != hide) {
        d->hideSpecificFolder = hide;
        invalidate();
    }
}

bool FolderTreeWidgetProxyModel::hideSpecificFolder() const
{
    return d->hideSpecificFolder;
}

void FolderTreeWidgetProxyModel::setHideOutboxFolder(bool hide)
{
    if (d->hideOutboxFolder != hide) {
        d->hideOutboxFolder = hide;
        invalidate();
    }
}

bool FolderTreeWidgetProxyModel::hideOutboxFolder() const
{
    return d->hideOutboxFolder;
}

bool FolderTreeWidgetProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex modelIndex = sourceModel()->index(sourceRow, 0, sourceParent);

    const auto collection = sourceModel()->data(modelIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
    if (!d->checker.isWantedCollection(collection)) {
        return false;
    }

    if (d->hideVirtualFolder) {
        if (Util::isVirtualCollection(collection)) {
            return false;
        }
    }

    if (d->hideSpecificFolder) {
        const QSharedPointer<FolderSettings> col = FolderSettings::forCollection(collection, false);
        if (col && col->hideInSelectionDialog()) {
            return false;
        }
    }

    if (d->hideOutboxFolder) {
        if (collection == Kernel::self()->outboxCollectionFolder()) {
            return false;
        }
    }

    return EntityRightsFilterModel::filterAcceptsRow(sourceRow, sourceParent);
}

QVariant FolderTreeWidgetProxyModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::ForegroundRole) {
        const QModelIndex sourceIndex = mapToSource(index);
        const QModelIndex rowIndex = sourceIndex.sibling(sourceIndex.row(), 0);
        const auto collection = sourceModel()->data(rowIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();

        if (!MailCommon::Util::isVirtualCollection(collection)) {
            const Akonadi::AgentInstance instance = Akonadi::AgentManager::self()->instance(collection.resource());

            if (instance.status() == Akonadi::AgentInstance::Broken) {
                if (!d->brokenAccountColor.isValid()) {
                    const KColorScheme scheme(QPalette::Active, KColorScheme::View);
                    d->brokenAccountColor = scheme.foreground(KColorScheme::NegativeText).color();
                }
                return d->brokenAccountColor;
            }
        }
    } else if (role == Qt::DisplayRole) {
        const QModelIndex sourceIndex = mapToSource(index);
        const QModelIndex rowIndex = sourceIndex.sibling(sourceIndex.row(), 0);
        const auto collection = sourceModel()->data(rowIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        if (!MailCommon::Util::isVirtualCollection(collection)) {
            const Akonadi::AgentInstance instance = Akonadi::AgentManager::self()->instance(collection.resource());
            if (collection.parentCollection() == Akonadi::Collection::root()) {
                if (!instance.isOnline()) {
                    return i18n("%1 (Offline)", Akonadi::EntityRightsFilterModel::data(index, role).toString());
                }
                qreal percentage = 0.0;
                if (d->checkQuotaExcedded(index, percentage)) {
                    return i18n("%1 (Reached %2% quota)", Akonadi::EntityRightsFilterModel::data(index, role).toString(), static_cast<int>(percentage));
                }
            }
        }
    } else if (role == Qt::DecorationRole) {
        const QModelIndex sourceIndex = mapToSource(index);
        const QModelIndex rowIndex = sourceIndex.sibling(sourceIndex.row(), 0);
        const auto collection = sourceModel()->data(rowIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        if (!MailCommon::Util::isVirtualCollection(collection)) {
            if (collection.parentCollection() == Akonadi::Collection::root()) {
                qreal percentage = 0.0;
                if (d->checkQuotaExcedded(index, percentage)) {
                    return QIcon::fromTheme(QStringLiteral("emblem-warning"));
                }
            }
        }
    }
    return Akonadi::EntityRightsFilterModel::data(index, role);
}

void FolderTreeWidgetProxyModel::updatePalette()
{
    if (d->brokenAccountColor.isValid()) {
        KColorScheme scheme(QPalette::Active, KColorScheme::View);
        d->brokenAccountColor = scheme.foreground(KColorScheme::NegativeText).color();
        invalidate();
    }
}

void FolderTreeWidgetProxyModel::addContentMimeTypeInclusionFilter(const QString &mimeType)
{
    d->includedMimeTypes << mimeType;
    d->checker.setWantedMimeTypes(d->includedMimeTypes.values());
    invalidateFilter();
}
}
