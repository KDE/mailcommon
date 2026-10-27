/*

  SPDX-FileCopyrightText: 2009-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "foldertreewidget.h"
#include "mailcommon_export.h"

#include <Akonadi/Collection>
#include <Akonadi/EntityTreeView>

class QMouseEvent;

namespace Akonadi
{
class CollectionStatisticsDelegate;
}

namespace MailCommon
{
/*!
 * \class MailCommon::FolderTreeView
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FolderTreeView
 *
 * This is an enhanced EntityTreeView specially suited for the folders in KMail's
 * main folder widget.
 */
class MAILCOMMON_EXPORT FolderTreeView : public Akonadi::EntityTreeView
{
    Q_OBJECT

public:
    /*!
     * Constructs a folder tree view with the given \a parent.
     * The unread count is shown if \a showUnreadCount is \c true.
     */
    explicit FolderTreeView(QWidget *parent = nullptr, bool showUnreadCount = true);

    /*!
     * Constructs a folder tree view using the \a xmlGuiClient for its actions, with the given \a parent.
     * The unread count is shown if \a showUnreadCount is \c true.
     */
    explicit FolderTreeView(KXMLGUIClient *xmlGuiClient, QWidget *parent = nullptr, bool showUnreadCount = true);

    /*!
     * Destroys the folder tree view.
     */
    ~FolderTreeView() override;

    /*!
     * Selects the next folder with unread messages.
     * If \a confirm is \c true, the user is asked before going to the next folder, and the drafts, sent and templates folders are skipped.
     */
    void selectNextUnreadFolder(bool confirm = false);
    /*!
     * Selects the previous folder with unread messages.
     * If \a confirm is \c true, the user is asked before going to the previous folder, and the drafts, sent and templates folders are skipped.
     */
    void selectPrevUnreadFolder(bool confirm = false);

    /*!
     * Enables the progress animation of the collection statistics if \a anim is \c true.
     */
    void showStatisticAnimation(bool anim);

    /*!
     * Disables the context menu and hides all the columns except the first one.
     */
    void disableContextMenuAndExtraColumn();

    /*!
     * Sets the tooltip display policy.
     */
    void setTooltipsPolicy(FolderTreeWidget::ToolTipDisplayPolicy);

    /*!
     * Restores the header state from \a data.
     */
    void restoreHeaderState(const QByteArray &data);

    /*!
     * Returns the current folder.
     */
    [[nodiscard]] Akonadi::Collection currentFolder() const;

    /*!
     * Disables saving the configuration of the view.
     */
    void disableSaveConfig();
    /*!
     * Reads the configuration of the view.
     */
    void readConfig();

    /*!
     * Updates the palette used to paint the collection statistics.
     */
    void updatePalette();

    /*!
     * Reimplemented to disable the keyboard search.
     */
    void keyboardSearch(const QString &) override;

    /*!
     * Enables drag and drop if \a enabled is \c true.
     */
    void setEnableDragDrop(bool enabled);

protected:
    enum Move {
        Next = 0,
        Previous = 1,
    };

    /*!
     * Initializes the view. The unread count is shown if \a showUnreadCount is \c true.
     */
    void init(bool showUnreadCount);
    /*!
     * Selects the given index.
     */
    void selectModelIndex(const QModelIndex &);
    /*!
     * Sets the given index as current index.
     */
    void setCurrentModelIndex(const QModelIndex &);
    /*!
     * Returns the index of the folder after \a current, expanding \a current if it has children.
     */
    QModelIndex selectNextFolder(const QModelIndex &current);
    /*!
     * Looks for the folder next to \a current in the direction \a move and stores its index in \a nextIndex.
     * Returns \c true if that folder has unread messages and was selected.
     * If \a confirm is \c true, the user is asked before going to that folder.
     */
    bool isUnreadFolder(const QModelIndex &current, QModelIndex &nextIndex, FolderTreeView::Move move, bool confirm);
    /*!
     * Writes the configuration of the view.
     */
    void writeConfig();

    /*!
     * Sets the sorting \a policy. If \a writeInConfig is \c true, the policy is saved in the configuration.
     */
    void setSortingPolicy(FolderTreeWidget::SortingPolicy policy, bool writeInConfig = false);

    /*!
     * Reimplemented to handle the mouse press event \a e.
     */
    void mousePressEvent(QMouseEvent *e) override;

public Q_SLOTS:
    /*!
     * Moves the focus to the next folder.
     */
    void slotFocusNextFolder();
    /*!
     * Moves the focus to the previous folder.
     */
    void slotFocusPrevFolder();
    /*!
     * Selects the focused folder.
     */
    void slotSelectFocusFolder();
    /*!
     * Moves the focus to the first folder.
     */
    void slotFocusFirstFolder();
    /*!
     * Moves the focus to the last folder.
     */
    void slotFocusLastFolder();

protected Q_SLOTS:
    /*!
     * Shows the header context menu.
     */
    void slotHeaderContextMenuRequested(const QPoint &);
    /*!
     * Changes the icon size from the header context menu.
     */
    void slotHeaderContextMenuChangeIconSize(bool);
    /*!
     * Shows or hides a column from the header context menu.
     */
    void slotHeaderContextMenuChangeHeader(bool);
    /*!
     * Changes the tooltip display policy from the header context menu.
     */
    void slotHeaderContextMenuChangeToolTipDisplayPolicy(bool);
    /*!
     * Changes the sorting policy from the header context menu.
     */
    void slotHeaderContextMenuChangeSortingPolicy(bool);

Q_SIGNALS:
    /*!
     * Emitted when the tooltip display policy changes.
     */
    void changeTooltipsPolicy(MailCommon::FolderTreeWidget::ToolTipDisplayPolicy);
    /*!
     * Emitted when manual sorting is enabled or disabled, according to \a actif.
     */
    void manualSortingChanged(bool actif);
    /*!
     * Emitted when a new tab is requested.
     */
    void newTabRequested(bool);

private:
    enum SearchDirection {
        ForwardSearch,
        BackwardSearch,
    };

    MAILCOMMON_NO_EXPORT QModelIndex indexAbove(const QModelIndex &current) const;
    MAILCOMMON_NO_EXPORT QModelIndex indexBelow(const QModelIndex &current) const;
    MAILCOMMON_NO_EXPORT QModelIndex lastChild(const QModelIndex &current) const;
    MAILCOMMON_NO_EXPORT QModelIndex nextUnreadCollection(const QModelIndex &current, SearchDirection direction) const;

    MAILCOMMON_NO_EXPORT bool ignoreUnreadFolder(const Akonadi::Collection &, bool) const;
    MAILCOMMON_NO_EXPORT bool allowedToEnterFolder(const Akonadi::Collection &, bool) const;
    MAILCOMMON_NO_EXPORT bool trySelectNextUnreadFolder(const QModelIndex &, SearchDirection, bool);

    FolderTreeWidget::ToolTipDisplayPolicy mToolTipDisplayPolicy;
    FolderTreeWidget::SortingPolicy mSortingPolicy;
    Akonadi::CollectionStatisticsDelegate *mCollectionStatisticsDelegate = nullptr;
    bool mbDisableContextMenuAndExtraColumn = false;
    bool mbDisableSaveConfig = false;
};
}
