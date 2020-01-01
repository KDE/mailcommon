/*

  Copyright (c) 2009-2020 Laurent Montel <montel@kde.org>

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

#ifndef MAILCOMMON_FOLDERTREEVIEW_H
#define MAILCOMMON_FOLDERTREEVIEW_H

#include "mailcommon_export.h"
#include "foldertreewidget.h"
#include "mailcommon/mailutil.h"

#include <EntityTreeView>
#include <Collection>

class QMouseEvent;

namespace Akonadi {
class CollectionStatisticsDelegate;
}

namespace MailCommon {
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

    Q_REQUIRED_RESULT Akonadi::Collection currentFolder() const;

    void disableSaveConfig();
    void readConfig();

    void updatePalette();

    void keyboardSearch(const QString &) override;
protected:
    enum Move {
        Next = 0,
        Previous = 1
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
        BackwardSearch
    };

    QModelIndex indexAbove(const QModelIndex &current) const;
    QModelIndex indexBelow(const QModelIndex &current) const;
    QModelIndex lastChild(const QModelIndex &current) const;
    QModelIndex nextUnreadCollection(const QModelIndex &current, SearchDirection direction) const;

    bool ignoreUnreadFolder(const Akonadi::Collection &, bool) const;
    bool allowedToEnterFolder(const Akonadi::Collection &, bool) const;
    bool trySelectNextUnreadFolder(const QModelIndex &, SearchDirection, bool);

    FolderTreeWidget::ToolTipDisplayPolicy mToolTipDisplayPolicy;
    FolderTreeWidget::SortingPolicy mSortingPolicy;
    Akonadi::CollectionStatisticsDelegate *mCollectionStatisticsDelegate = nullptr;
    bool mbDisableContextMenuAndExtraColumn = false;
    bool mbDisableSaveConfig = false;
};
}

#endif
