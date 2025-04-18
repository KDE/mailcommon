/*

  SPDX-FileCopyrightText: 2009-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "foldertreewidget.h"
#include "mailcommon/mailutil.h"
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
/**
 * This is an enhanced EntityTreeView specially suited for the folders in KMail's
 * main folder widget.
 */
class MAILCOMMON_EXPORT FolderTreeView : public Akonadi::EntityTreeView
{
    Q_OBJECT

public:
    explicit FolderTreeView(QWidget *parent = nullptr, bool showUnreadCount = true);

    explicit FolderTreeView(KXMLGUIClient *xmlGuiClient, QWidget *parent = nullptr, bool showUnreadCount = true);

    ~FolderTreeView() override;

    void selectNextUnreadFolder(bool confirm = false);
    void selectPrevUnreadFolder(bool confirm = false);

    void showStatisticAnimation(bool anim);

    void disableContextMenuAndExtraColumn();

    void setTooltipsPolicy(FolderTreeWidget::ToolTipDisplayPolicy);

    void restoreHeaderState(const QByteArray &data);

    [[nodiscard]] Akonadi::Collection currentFolder() const;

    void disableSaveConfig();
    void readConfig();

    void updatePalette();

    void keyboardSearch(const QString &) override;

    void setEnableDragDrop(bool enabled);

protected:
    enum Move {
        Next = 0,
        Previous = 1,
    };

    void init(bool showUnreadCount);
    void selectModelIndex(const QModelIndex &);
    void setCurrentModelIndex(const QModelIndex &);
    QModelIndex selectNextFolder(const QModelIndex &current);
    bool isUnreadFolder(const QModelIndex &current, QModelIndex &nextIndex, FolderTreeView::Move move, bool confirm);
    void writeConfig();

    void setSortingPolicy(FolderTreeWidget::SortingPolicy policy, bool writeInConfig = false);

    void mousePressEvent(QMouseEvent *e) override;

public Q_SLOTS:
    void slotFocusNextFolder();
    void slotFocusPrevFolder();
    void slotSelectFocusFolder();
    void slotFocusFirstFolder();
    void slotFocusLastFolder();

protected Q_SLOTS:
    void slotHeaderContextMenuRequested(const QPoint &);
    void slotHeaderContextMenuChangeIconSize(bool);
    void slotHeaderContextMenuChangeHeader(bool);
    void slotHeaderContextMenuChangeToolTipDisplayPolicy(bool);
    void slotHeaderContextMenuChangeSortingPolicy(bool);

Q_SIGNALS:
    void changeTooltipsPolicy(FolderTreeWidget::ToolTipDisplayPolicy);
    void manualSortingChanged(bool actif);
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
