/*

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "foldertreeview.h"
#include "kernel/mailkernel.h"
#include "util/mailutil_p.h"

#include <CollectionStatistics>
#include <CollectionStatisticsDelegate>
#include <EntityTreeModel>

#include <KConfigGroup>
#include <KGuiItem>
#include <KLocalizedString>
#include <KMessageBox>
#include <QMenu>

#include <QHeaderView>
#include <QMouseEvent>

using namespace MailCommon;

FolderTreeView::FolderTreeView(QWidget *parent, bool showUnreadCount)
    : Akonadi::EntityTreeView(parent)
{
    init(showUnreadCount);
}

FolderTreeView::FolderTreeView(KXMLGUIClient *xmlGuiClient, QWidget *parent, bool showUnreadCount)
    : Akonadi::EntityTreeView(xmlGuiClient, parent)
{
    init(showUnreadCount);
}

FolderTreeView::~FolderTreeView()
{
}

void FolderTreeView::disableSaveConfig()
{
    mbDisableSaveConfig = true;
}

void FolderTreeView::setTooltipsPolicy(FolderTreeWidget::ToolTipDisplayPolicy policy)
{
    if (mToolTipDisplayPolicy == policy) {
        return;
    }

    mToolTipDisplayPolicy = policy;
    Q_EMIT changeTooltipsPolicy(mToolTipDisplayPolicy);
    writeConfig();
}

void FolderTreeView::disableContextMenuAndExtraColumn()
{
    mbDisableContextMenuAndExtraColumn = true;
    const int nbColumn = header()->count();
    for (int i = 1; i < nbColumn; ++i) {
        setColumnHidden(i, true);
    }
}

void FolderTreeView::init(bool showUnreadCount)
{
    setIconSize(QSize(22, 22));
    setUniformRowHeights(true);
    mSortingPolicy = FolderTreeWidget::SortByCurrentColumn;
    mToolTipDisplayPolicy = FolderTreeWidget::DisplayAlways;

    header()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(header(), &QWidget::customContextMenuRequested, this, &FolderTreeView::slotHeaderContextMenuRequested);

    mCollectionStatisticsDelegate = new Akonadi::CollectionStatisticsDelegate(this);
    mCollectionStatisticsDelegate->setProgressAnimationEnabled(true);
    setItemDelegate(mCollectionStatisticsDelegate);
    mCollectionStatisticsDelegate->setUnreadCountShown(showUnreadCount && !header()->isSectionHidden(1));
}

void FolderTreeView::showStatisticAnimation(bool anim)
{
    mCollectionStatisticsDelegate->setProgressAnimationEnabled(anim);
}

void FolderTreeView::writeConfig()
{
    if (mbDisableSaveConfig) {
        return;
    }

    KConfigGroup myGroup(KernelIf->config(), "MainFolderView");
    myGroup.writeEntry("IconSize", iconSize().width());
    myGroup.writeEntry("ToolTipDisplayPolicy", (int)mToolTipDisplayPolicy);
    myGroup.writeEntry("SortingPolicy", (int)mSortingPolicy);
}

void FolderTreeView::readConfig()
{
    KConfigGroup myGroup(KernelIf->config(), "MainFolderView");
    int iIconSize = myGroup.readEntry("IconSize", iconSize().width());
    if ((iIconSize < 16) || (iIconSize > 32)) {
        iIconSize = 22;
    }
    setIconSize(QSize(iIconSize, iIconSize));
    mToolTipDisplayPolicy =
        static_cast<FolderTreeWidget::ToolTipDisplayPolicy>(myGroup.readEntry("ToolTipDisplayPolicy", static_cast<int>(FolderTreeWidget::DisplayAlways)));

    Q_EMIT changeTooltipsPolicy(mToolTipDisplayPolicy);

    setSortingPolicy((FolderTreeWidget::SortingPolicy)myGroup.readEntry("SortingPolicy", (int)FolderTreeWidget::SortByCurrentColumn), false);
}

void FolderTreeView::slotHeaderContextMenuRequested(const QPoint &pnt)
{
    if (mbDisableContextMenuAndExtraColumn) {
        readConfig();
        return;
    }

    // the menu for the columns
    QMenu menu;
    QAction *act = nullptr;
    const int nbColumn = header()->count();
    if (nbColumn > 1) {
        menu.addSection(i18n("View Columns"));
        for (int i = 1; i < nbColumn; ++i) {
            act = menu.addAction(model()->headerData(i, Qt::Horizontal).toString());
            act->setCheckable(true);
            act->setChecked(!header()->isSectionHidden(i));
            act->setData(QVariant(i));
            connect(act, &QAction::triggered, this, &FolderTreeView::slotHeaderContextMenuChangeHeader);
        }
    }

    menu.addSection(i18n("Icon Size"));

    static const int icon_sizes[] = {16, 22, 32};

    auto grp = new QActionGroup(&menu);
    for (int i : icon_sizes) {
        act = menu.addAction(QStringLiteral("%1x%2").arg(i).arg(i));
        act->setCheckable(true);
        grp->addAction(act);
        if (iconSize().width() == i) {
            act->setChecked(true);
        }
        act->setData(QVariant(i));

        connect(act, &QAction::triggered, this, &FolderTreeView::slotHeaderContextMenuChangeIconSize);
    }
    menu.addSection(i18n("Display Tooltips"));

    grp = new QActionGroup(&menu);

    act = menu.addAction(i18nc("@action:inmenu Always display tooltips", "Always"));
    act->setCheckable(true);
    grp->addAction(act);
    act->setChecked(mToolTipDisplayPolicy == FolderTreeWidget::DisplayAlways);
    act->setData(QVariant((int)FolderTreeWidget::DisplayAlways));
    connect(act, &QAction::triggered, this, &FolderTreeView::slotHeaderContextMenuChangeToolTipDisplayPolicy);

    act = menu.addAction(i18nc("@action:inmenu Never display tooltips.", "Never"));
    act->setCheckable(true);
    grp->addAction(act);
    act->setChecked(mToolTipDisplayPolicy == FolderTreeWidget::DisplayNever);
    act->setData(QVariant((int)FolderTreeWidget::DisplayNever));
    connect(act, &QAction::triggered, this, &FolderTreeView::slotHeaderContextMenuChangeToolTipDisplayPolicy);

    menu.addSection(i18nc("@action:inmenu", "Sort Items"));

    grp = new QActionGroup(&menu);

    act = menu.addAction(i18nc("@action:inmenu", "Automatically, by Current Column"));
    act->setCheckable(true);
    grp->addAction(act);
    act->setChecked(mSortingPolicy == FolderTreeWidget::SortByCurrentColumn);
    act->setData(QVariant((int)FolderTreeWidget::SortByCurrentColumn));
    connect(act, &QAction::triggered, this, &FolderTreeView::slotHeaderContextMenuChangeSortingPolicy);

    act = menu.addAction(i18nc("@action:inmenu", "Manually, by Drag And Drop"));
    act->setCheckable(true);
    grp->addAction(act);
    act->setChecked(mSortingPolicy == FolderTreeWidget::SortByDragAndDropKey);
    act->setData(QVariant((int)FolderTreeWidget::SortByDragAndDropKey));
    connect(act, &QAction::triggered, this, &FolderTreeView::slotHeaderContextMenuChangeSortingPolicy);

    menu.exec(header()->mapToGlobal(pnt));
}

void FolderTreeView::slotHeaderContextMenuChangeSortingPolicy(bool)
{
    auto act = qobject_cast<QAction *>(sender());
    if (!act) {
        return;
    }

    QVariant data = act->data();

    bool ok;
    int policy = data.toInt(&ok);
    if (!ok) {
        return;
    }

    setSortingPolicy((FolderTreeWidget::SortingPolicy)policy, true);
}

void FolderTreeView::setSortingPolicy(FolderTreeWidget::SortingPolicy policy, bool writeInConfig)
{
    if (mSortingPolicy == policy) {
        return;
    }

    mSortingPolicy = policy;
    switch (mSortingPolicy) {
    case FolderTreeWidget::SortByCurrentColumn:
        header()->setSectionsClickable(true);
        header()->setSortIndicatorShown(true);
        setSortingEnabled(true);
        Q_EMIT manualSortingChanged(false);
        break;

    case FolderTreeWidget::SortByDragAndDropKey:
        header()->setSectionsClickable(false);
        header()->setSortIndicatorShown(false);

        setSortingEnabled(false); // hack for qutie bug: this call shouldn't be here at all
        Q_EMIT manualSortingChanged(true);

        break;
    default:
        // should never happen
        break;
    }
    if (writeInConfig) {
        writeConfig();
    }
}

void FolderTreeView::slotHeaderContextMenuChangeToolTipDisplayPolicy(bool)
{
    auto act = qobject_cast<QAction *>(sender());
    if (!act) {
        return;
    }

    QVariant data = act->data();

    bool ok;
    const int id = data.toInt(&ok);
    if (!ok) {
        return;
    }
    Q_EMIT changeTooltipsPolicy((FolderTreeWidget::ToolTipDisplayPolicy)id);
}

void FolderTreeView::slotHeaderContextMenuChangeHeader(bool)
{
    auto act = qobject_cast<QAction *>(sender());
    if (!act) {
        return;
    }

    QVariant data = act->data();

    bool ok;
    const int id = data.toInt(&ok);
    if (!ok) {
        return;
    }

    if (id >= header()->count()) {
        return;
    }

    if (id == 1) {
        mCollectionStatisticsDelegate->setUnreadCountShown(!act->isChecked());
    }

    setColumnHidden(id, !act->isChecked());
}

void FolderTreeView::slotHeaderContextMenuChangeIconSize(bool)
{
    auto act = qobject_cast<QAction *>(sender());
    if (!act) {
        return;
    }

    QVariant data = act->data();

    bool ok;
    const int size = data.toInt(&ok);
    if (!ok) {
        return;
    }

    const QSize newIconSize(QSize(size, size));
    if (newIconSize == iconSize()) {
        return;
    }
    setIconSize(newIconSize);

    writeConfig();
}

void FolderTreeView::setCurrentModelIndex(const QModelIndex &index)
{
    if (index.isValid()) {
        clearSelection();
        scrollTo(index);
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::Rows);
    }
}

void FolderTreeView::selectModelIndex(const QModelIndex &index)
{
    if (index.isValid()) {
        scrollTo(index);
        selectionModel()->select(index, QItemSelectionModel::Rows | QItemSelectionModel::Select | QItemSelectionModel::Current | QItemSelectionModel::Clear);
    }
}

void FolderTreeView::slotSelectFocusFolder()
{
    const QModelIndex index = currentIndex();
    if (index.isValid()) {
        setCurrentIndex(index);
    }
}

void FolderTreeView::slotFocusNextFolder()
{
    const QModelIndex nextFolder = selectNextFolder(currentIndex());

    if (nextFolder.isValid()) {
        expand(nextFolder);
        setCurrentModelIndex(nextFolder);
    }
}

QModelIndex FolderTreeView::selectNextFolder(const QModelIndex &current)
{
    QModelIndex below;
    if (current.isValid()) {
        model()->fetchMore(current);
        if (model()->hasChildren(current)) {
            expand(current);
            below = indexBelow(current);
        } else if (current.row() < model()->rowCount(model()->parent(current)) - 1) {
            below = model()->index(current.row() + 1, current.column(), model()->parent(current));
        } else {
            below = indexBelow(current);
        }
    }
    return below;
}

void FolderTreeView::slotFocusPrevFolder()
{
    const QModelIndex current = currentIndex();
    if (current.isValid()) {
        QModelIndex above = indexAbove(current);
        setCurrentModelIndex(above);
    }
}

void FolderTreeView::slotFocusFirstFolder()
{
    const QModelIndex first = moveCursor(QAbstractItemView::MoveHome, Qt::NoModifier);
    if (first.isValid()) {
        setCurrentModelIndex(first);
    }
}

void FolderTreeView::slotFocusLastFolder()
{
    const QModelIndex last = moveCursor(QAbstractItemView::MoveEnd, Qt::NoModifier);
    if (last.isValid()) {
        setCurrentModelIndex(last);
    }
}

void FolderTreeView::selectNextUnreadFolder(bool confirm)
{
    // find next unread collection starting from current position
    if (!trySelectNextUnreadFolder(currentIndex(), ForwardSearch, confirm)) {
        // if there is none, jump to the last collection and try again
        trySelectNextUnreadFolder(model()->index(0, 0), ForwardSearch, confirm);
    }
}

// helper method to find last item in the model tree
static QModelIndex lastChildOf(QAbstractItemModel *model, const QModelIndex &current)
{
    if (model->rowCount(current) == 0) {
        return current;
    }

    return lastChildOf(model, model->index(model->rowCount(current) - 1, 0, current));
}

void FolderTreeView::selectPrevUnreadFolder(bool confirm)
{
    // find next unread collection starting from current position
    if (!trySelectNextUnreadFolder(currentIndex(), BackwardSearch, confirm)) {
        // if there is none, jump to top and try again
        const QModelIndex index = lastChildOf(model(), QModelIndex());
        trySelectNextUnreadFolder(index, BackwardSearch, confirm);
    }
}

bool FolderTreeView::trySelectNextUnreadFolder(const QModelIndex &current, SearchDirection direction, bool confirm)
{
    QModelIndex index = current;
    while (true) {
        index = nextUnreadCollection(index, direction);

        if (!index.isValid()) {
            return false;
        }

        const auto collection = index.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        if (collection == Kernel::self()->trashCollectionFolder() || collection == Kernel::self()->outboxCollectionFolder()) {
            continue;
        }

        if (ignoreUnreadFolder(collection, confirm)) {
            continue;
        }

        if (allowedToEnterFolder(collection, confirm)) {
            expand(index);
            setCurrentIndex(index);
            selectModelIndex(index);
            return true;
        } else {
            return false;
        }
    }

    return false;
}

bool FolderTreeView::ignoreUnreadFolder(const Akonadi::Collection &collection, bool confirm) const
{
    if (!confirm) {
        return false;
    }

    // Skip drafts, sent mail and templates as well, when reading mail with the
    // space bar - but not when changing into the next folder with unread mail
    // via ctrl+ or ctrl- so we do this only if (confirm == true), which means
    // we are doing readOn.

    return collection == Kernel::self()->draftsCollectionFolder() || collection == Kernel::self()->templatesCollectionFolder()
        || collection == Kernel::self()->sentCollectionFolder();
}

bool FolderTreeView::allowedToEnterFolder(const Akonadi::Collection &collection, bool confirm) const
{
    if (!confirm) {
        return true;
    }

    // warn user that going to next folder - but keep track of
    // whether he wishes to be notified again in "AskNextFolder"
    // parameter (kept in the config file for kmail)
    const int result = KMessageBox::questionYesNo(const_cast<FolderTreeView *>(this),
                                                  i18n("<qt>Go to the next unread message in folder <b>%1</b>?</qt>", collection.name()),
                                                  i18n("Go to Next Unread Message"),
                                                  KGuiItem(i18n("Go To")),
                                                  KGuiItem(i18n("Do Not Go To")), // defaults
                                                  QStringLiteral(":kmail_AskNextFolder"),
                                                  KMessageBox::Option());

    return result == KMessageBox::Yes;
}

bool FolderTreeView::isUnreadFolder(const QModelIndex &current, QModelIndex &index, FolderTreeView::Move move, bool confirm)
{
    if (current.isValid()) {
        if (move == FolderTreeView::Next) {
            index = selectNextFolder(current);
        } else if (move == FolderTreeView::Previous) {
            index = indexAbove(current);
        }

        if (index.isValid()) {
            const auto collection = index.model()->data(current, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();

            if (collection.isValid()) {
                if (collection.statistics().unreadCount() > 0) {
                    if (!confirm) {
                        selectModelIndex(current);
                        return true;
                    } else {
                        // Skip drafts, sent mail and templates as well, when reading mail with the
                        // space bar - but not when changing into the next folder with unread mail
                        // via ctrl+ or ctrl- so we do this only if (confirm == true), which means
                        // we are doing readOn.

                        if (collection == Kernel::self()->draftsCollectionFolder() || collection == Kernel::self()->templatesCollectionFolder()
                            || collection == Kernel::self()->sentCollectionFolder()) {
                            return false;
                        }

                        // warn user that going to next folder - but keep track of
                        // whether he wishes to be notified again in "AskNextFolder"
                        // parameter (kept in the config file for kmail)
                        if (KMessageBox::questionYesNo(this,
                                                       i18n("<qt>Go to the next unread message in folder <b>%1</b>?</qt>", collection.name()),
                                                       i18n("Go to Next Unread Message"),
                                                       KGuiItem(i18n("Go To")),
                                                       KGuiItem(i18n("Do Not Go To")), // defaults
                                                       QStringLiteral(":kmail_AskNextFolder"),
                                                       KMessageBox::Option())
                            == KMessageBox::No) {
                            return true; // assume selected (do not continue looping)
                        }

                        selectModelIndex(current);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

Akonadi::Collection FolderTreeView::currentFolder() const
{
    const QModelIndex current = currentIndex();
    if (current.isValid()) {
        const auto collection = current.model()->data(current, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        return collection;
    }
    return Akonadi::Collection();
}

void FolderTreeView::mousePressEvent(QMouseEvent *e)
{
    const bool buttonPressedIsMiddle = (e->button() == Qt::MiddleButton);
    Q_EMIT newTabRequested(buttonPressedIsMiddle);
    EntityTreeView::mousePressEvent(e);
}

void FolderTreeView::restoreHeaderState(const QByteArray &data)
{
    if (data.isEmpty()) {
        const int nbColumn = header()->count();
        for (int i = 1; i < nbColumn; ++i) {
            setColumnHidden(i, true);
        }
    } else {
        header()->restoreState(data);
    }
    mCollectionStatisticsDelegate->setUnreadCountShown(header()->isSectionHidden(1));
}

void FolderTreeView::updatePalette()
{
    mCollectionStatisticsDelegate->updatePalette();
}

void FolderTreeView::keyboardSearch(const QString &)
{
    // Disable keyboardSearch: it interfers with filtering in the
    // FolderSelectionDialog. We don't want it in KMail main window
    // either because KMail has one-letter keyboard shortcuts.
}

QModelIndex FolderTreeView::indexBelow(const QModelIndex &current) const
{
    // if we have children, return first child
    if (model()->rowCount(current) > 0) {
        return model()->index(0, 0, current);
    }

    // if we have siblings, return next sibling
    const QModelIndex parent = model()->parent(current);
    const QModelIndex sibling = model()->index(current.row() + 1, 0, parent);

    if (sibling.isValid()) { // found valid sibling
        return sibling;
    }

    if (!parent.isValid()) { // our parent is the tree root and we have no siblings
        return QModelIndex(); // we reached the bottom of the tree
    }

    // We are the last child, the next index to check is our uncle, parent's first sibling
    const QModelIndex parentsSibling = parent.sibling(parent.row() + 1, 0);
    if (parentsSibling.isValid()) {
        return parentsSibling;
    }

    // iterate over our parents back to root until we find a parent with a valid sibling
    QModelIndex currentParent = parent;
    QModelIndex grandParent = model()->parent(currentParent);
    while (currentParent.isValid()) {
        // check if the parent has children except from us
        if (model()->rowCount(grandParent) > currentParent.row() + 1) {
            const auto index = indexBelow(model()->index(currentParent.row() + 1, 0, grandParent));
            if (index.isValid()) {
                return index;
            }
        }

        currentParent = grandParent;
        grandParent = model()->parent(currentParent);
    }

    return QModelIndex(); // nothing found -> end of tree
}

QModelIndex FolderTreeView::lastChild(const QModelIndex &current) const
{
    if (model()->rowCount(current) == 0) {
        return current;
    }

    return lastChild(model()->index(model()->rowCount(current) - 1, 0, current));
}

QModelIndex FolderTreeView::indexAbove(const QModelIndex &current) const
{
    const QModelIndex parent = model()->parent(current);

    if (current.row() == 0) {
        // we have no previous siblings -> our parent is the next item above us
        return parent;
    }

    // find previous sibling
    const QModelIndex previousSibling = model()->index(current.row() - 1, 0, parent);

    // the item above us is the last child (or grandchild, or grandgrandchild... etc)
    // of our previous sibling
    return lastChild(previousSibling);
}

QModelIndex FolderTreeView::nextUnreadCollection(const QModelIndex &current, SearchDirection direction) const
{
    QModelIndex index = current;
    while (true) {
        if (direction == ForwardSearch) {
            index = indexBelow(index);
        } else if (direction == BackwardSearch) {
            index = indexAbove(index);
        }

        if (!index.isValid()) { // reach end or top of the model
            return QModelIndex();
        }

        // check if the index is a collection
        const auto collection = index.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();

        if (collection.isValid()) {
            // check if it is unread
            if (collection.statistics().unreadCount() > 0) {
                if (!MailCommon::Util::ignoreNewMailInFolder(collection)) {
                    return index; // we found the next unread collection
                }
            }
        }
    }

    return QModelIndex(); // no unread collection found
}
