/*

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "foldertreewidget.h"
#include "entitycollectionorderproxymodel.h"
#include "foldertreeview.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"

#include <PimCommon/PimUtil>
#include <PimCommonAkonadi/ImapAclAttribute>

#include <AkonadiCore/AttributeFactory>
#include <AkonadiCore/ChangeRecorder>
#include <AkonadiCore/Collection>
#include <AkonadiCore/EntityMimeTypeFilterModel>
#include <AkonadiCore/EntityTreeModel>
#include <AkonadiCore/ItemFetchScope>
#include <AkonadiCore/StatisticsProxyModel>

#include <AkonadiWidgets/ETMViewStateSaver>
#include <AkonadiWidgets/EntityTreeView>

#include <KMime/Message>

#include <MessageCore/MessageCoreSettings>

#include <KLocalizedString>

#include <QFontDatabase>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPointer>
#include <QVBoxLayout>

namespace MailCommon
{
class Q_DECL_HIDDEN FolderTreeWidget::FolderTreeWidgetPrivate
{
public:
    QString filter;
    QString oldFilterStr;
    Akonadi::StatisticsProxyModel *filterModel = nullptr;
    FolderTreeView *folderTreeView = nullptr;
    FolderTreeWidgetProxyModel *readableproxy = nullptr;
    EntityCollectionOrderProxyModel *entityOrderProxy = nullptr;
    QLineEdit *filterFolderLineEdit = nullptr;
    QPointer<Akonadi::ETMViewStateSaver> saver;
    QStringList expandedItems;
    QString currentItem;
    QLabel *label = nullptr;
    bool dontKeyFilter = false;
};

FolderTreeWidget::FolderTreeWidget(QWidget *parent,
                                   KXMLGUIClient *xmlGuiClient,
                                   FolderTreeWidget::TreeViewOptions options,
                                   FolderTreeWidgetProxyModel::FolderTreeWidgetProxyModelOptions optReadableProxy)
    : QWidget(parent)
    , d(new FolderTreeWidgetPrivate())
{
    Akonadi::AttributeFactory::registerAttribute<PimCommon::ImapAclAttribute>();

    d->folderTreeView = new FolderTreeView(xmlGuiClient, this, options & ShowUnreadCount);
    d->folderTreeView->showStatisticAnimation(options & ShowCollectionStatisticAnimation);

    connect(d->folderTreeView, &FolderTreeView::manualSortingChanged, this, &FolderTreeWidget::slotManualSortingChanged);

    auto lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);

    d->label = new QLabel(i18n("You can start typing to filter the list of folders."), this);
    lay->addWidget(d->label);

    d->filterFolderLineEdit = new QLineEdit(this);

    d->filterFolderLineEdit->setClearButtonEnabled(true);
    d->filterFolderLineEdit->setPlaceholderText(i18nc("@info Displayed grayed-out inside the textbox, verb to search", "Search"));
    lay->addWidget(d->filterFolderLineEdit);

    if (!(options & HideStatistics)) {
        d->filterModel = new Akonadi::StatisticsProxyModel(this);
        d->filterModel->setSourceModel(KernelIf->collectionModel());
    }
    if (options & HideHeaderViewMenu) {
        d->folderTreeView->header()->setContextMenuPolicy(Qt::NoContextMenu);
    }

    d->readableproxy = new FolderTreeWidgetProxyModel(this, optReadableProxy);
    d->readableproxy->setSourceModel((options & HideStatistics) ? static_cast<QAbstractItemModel *>(KernelIf->collectionModel())
                                                                : static_cast<QAbstractItemModel *>(d->filterModel));
    d->readableproxy->addContentMimeTypeInclusionFilter(KMime::Message::mimeType());

    connect(d->folderTreeView, &FolderTreeView::changeTooltipsPolicy, this, &FolderTreeWidget::slotChangeTooltipsPolicy);

    d->folderTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    d->folderTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    d->folderTreeView->installEventFilter(this);

    // Order proxy
    d->entityOrderProxy = new EntityCollectionOrderProxyModel(this);
    d->entityOrderProxy->setSourceModel(d->readableproxy);
    d->entityOrderProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    KConfigGroup grp(KernelIf->config(), "CollectionTreeOrder");
    d->entityOrderProxy->setOrderConfig(grp);
    d->folderTreeView->setModel(d->entityOrderProxy);

    if (options & UseDistinctSelectionModel) {
        d->folderTreeView->setSelectionModel(new QItemSelectionModel(d->entityOrderProxy, this));
    }

    lay->addWidget(d->folderTreeView);

    d->dontKeyFilter = (options & DontKeyFilter);

    if ((options & UseLineEditForFiltering)) {
        connect(d->filterFolderLineEdit, &QLineEdit::textChanged, this, &FolderTreeWidget::slotFilterFixedString);
        d->label->hide();
    } else {
        d->filterFolderLineEdit->hide();
        setAttribute(Qt::WA_InputMethodEnabled);
    }
}

FolderTreeWidget::~FolderTreeWidget()
{
    delete d;
}

void FolderTreeWidget::slotFilterFixedString(const QString &text)
{
    delete d->saver;
    if (d->oldFilterStr.isEmpty()) {
        // Save it.
        Akonadi::ETMViewStateSaver saver;
        saver.setView(folderTreeView());
        d->expandedItems = saver.expansionKeys();
        d->currentItem = saver.currentIndexKey();
    } else if (text.isEmpty()) {
        d->saver = new Akonadi::ETMViewStateSaver;
        d->saver->setView(folderTreeView());
        QString currentIndex = d->saver->currentIndexKey();
        if (d->saver->selectionKeys().isEmpty()) {
            currentIndex = d->currentItem;
        } else if (!currentIndex.isEmpty()) {
            d->expandedItems << currentIndex;
        }
        d->saver->restoreExpanded(d->expandedItems);
        d->saver->restoreCurrentItem(currentIndex);
    } else {
        d->folderTreeView->expandAll();
    }
    d->oldFilterStr = text;
    d->entityOrderProxy->setFilterWildcard(text);
}

void FolderTreeWidget::disableContextMenuAndExtraColumn()
{
    d->folderTreeView->disableContextMenuAndExtraColumn();
}

void FolderTreeWidget::selectCollectionFolder(const Akonadi::Collection &collection)
{
    const QModelIndex index = Akonadi::EntityTreeModel::modelIndexForCollection(d->folderTreeView->model(), collection);

    d->folderTreeView->setCurrentIndex(index);
    d->folderTreeView->setExpanded(index, true);
    d->folderTreeView->scrollTo(index);
}

void FolderTreeWidget::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    d->folderTreeView->setSelectionMode(mode);
}

QAbstractItemView::SelectionMode FolderTreeWidget::selectionMode() const
{
    return d->folderTreeView->selectionMode();
}

QItemSelectionModel *FolderTreeWidget::selectionModel() const
{
    return d->folderTreeView->selectionModel();
}

QModelIndex FolderTreeWidget::currentIndex() const
{
    return d->folderTreeView->currentIndex();
}

Akonadi::Collection FolderTreeWidget::selectedCollection() const
{
    if (d->folderTreeView->selectionMode() == QAbstractItemView::SingleSelection) {
        Akonadi::Collection::List lstCollection = selectedCollections();
        if (lstCollection.isEmpty()) {
            return Akonadi::Collection();
        } else {
            return lstCollection.at(0);
        }
    }

    return Akonadi::Collection();
}

Akonadi::Collection::List FolderTreeWidget::selectedCollections() const
{
    Akonadi::Collection::List collections;
    const QItemSelectionModel *selectionModel = d->folderTreeView->selectionModel();
    const QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    for (const QModelIndex &index : selectedIndexes) {
        if (index.isValid()) {
            const auto collection = index.model()->data(index, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
            if (collection.isValid()) {
                collections.append(collection);
            }
        }
    }

    return collections;
}

FolderTreeView *FolderTreeWidget::folderTreeView() const
{
    return d->folderTreeView;
}

void FolderTreeWidget::slotGeneralFontChanged()
{
    // Custom/System font support
    if (MessageCore::MessageCoreSettings::self()->useDefaultFonts()) {
        setFont(QFontDatabase::systemFont(QFontDatabase::GeneralFont));
    }
}

void FolderTreeWidget::slotGeneralPaletteChanged()
{
    d->readableproxy->updatePalette();
    d->folderTreeView->updatePalette();
}

void FolderTreeWidget::readConfig()
{
    setFont(QFontDatabase::systemFont(QFontDatabase::GeneralFont));

    d->folderTreeView->readConfig();
    d->folderTreeView->setDropActionMenuEnabled(SettingsIf->showPopupAfterDnD());
    d->readableproxy->setWarningThreshold(SettingsIf->closeToQuotaThreshold());
    d->readableproxy->readConfig();

    KConfigGroup readerConfig(KernelIf->config(), "AccountOrder");
    QStringList listOrder;
    if (readerConfig.readEntry("EnableAccountOrder", true)) {
        listOrder = readerConfig.readEntry("order", QStringList());
    }
    d->entityOrderProxy->setTopLevelOrder(listOrder);
}

void FolderTreeWidget::restoreHeaderState(const QByteArray &data)
{
    d->folderTreeView->restoreHeaderState(data);
}

void FolderTreeWidget::slotChangeTooltipsPolicy(FolderTreeWidget::ToolTipDisplayPolicy policy)
{
    changeToolTipsPolicyConfig(policy);
}

void FolderTreeWidget::changeToolTipsPolicyConfig(ToolTipDisplayPolicy policy)
{
    switch (policy) {
    case DisplayAlways:
    case DisplayWhenTextElided: // Need to implement in the future
        if (d->filterModel) {
            d->filterModel->setToolTipEnabled(true);
        }
        break;
    case DisplayNever:
        if (d->filterModel) {
            d->filterModel->setToolTipEnabled(false);
        }
    }
    d->folderTreeView->setTooltipsPolicy(policy);
}

Akonadi::StatisticsProxyModel *FolderTreeWidget::statisticsProxyModel() const
{
    return d->filterModel;
}

FolderTreeWidgetProxyModel *FolderTreeWidget::folderTreeWidgetProxyModel() const
{
    return d->readableproxy;
}

EntityCollectionOrderProxyModel *FolderTreeWidget::entityOrderProxy() const
{
    return d->entityOrderProxy;
}

QLineEdit *FolderTreeWidget::filterFolderLineEdit() const
{
    return d->filterFolderLineEdit;
}

void FolderTreeWidget::applyFilter(const QString &filter)
{
    d->label->setText(filter.isEmpty() ? i18n("You can start typing to filter the list of folders.") : i18n("Path: (%1)", filter));

    d->entityOrderProxy->setFilterWildcard(filter);
    d->folderTreeView->expandAll();
    QAbstractItemModel *model = d->folderTreeView->model();
    QModelIndex current = d->folderTreeView->currentIndex();
    QModelIndex start = current.isValid() ? current : model->index(0, 0);
    QModelIndexList list = model->match(start, Qt::DisplayRole, d->filter, 1 /* stop at first hit */, Qt::MatchContains | Qt::MatchWrap | Qt::MatchRecursive);
    if (!list.isEmpty()) {
        current = list.first();
        d->folderTreeView->setCurrentIndex(current);
        d->folderTreeView->scrollTo(current);
    }
}

void FolderTreeWidget::clearFilter()
{
    d->filter.clear();
    applyFilter(d->filter);
    const QModelIndexList lst = d->folderTreeView->selectionModel()->selectedIndexes();
    if (!lst.isEmpty()) {
        d->folderTreeView->scrollTo(lst.first());
    }
}

void FolderTreeWidget::slotManualSortingChanged(bool active)
{
    d->entityOrderProxy->setManualSortingActive(active);
    d->folderTreeView->setManualSortingActive(active);
}

bool FolderTreeWidget::eventFilter(QObject *o, QEvent *e)
{
    Q_UNUSED(o)
    if (d->dontKeyFilter) {
        return false;
    }

    if (e->type() == QEvent::KeyPress) {
        const QKeyEvent *const ke = static_cast<QKeyEvent *>(e);
        switch (ke->key()) {
        case Qt::Key_Backspace: {
            const int filterLength(d->filter.length());
            if (filterLength > 0) {
                d->filter.truncate(filterLength - 1);
                applyFilter(d->filter);
            }
            return false;
        }
        case Qt::Key_Delete:
            d->filter.clear();
            applyFilter(d->filter);
            return false;
        default: {
            const QString s = ke->text();
            if (!s.isEmpty() && s.at(0).isPrint()) {
                d->filter += s;
                applyFilter(d->filter);
                return false;
            }
            break;
        }
        }
    } else if (e->type() == QEvent::InputMethod) {
        const QInputMethodEvent *const ime = static_cast<QInputMethodEvent *>(e);
        d->filter += ime->commitString();
        applyFilter(d->filter);
        return false;
    }
    return false;
}
}
