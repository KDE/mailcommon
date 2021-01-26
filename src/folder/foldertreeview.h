/*

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FOLDERTREEVIEW_H
#define MAILCOMMON_FOLDERTREEVIEW_H

#include "foldertreewidget.h"
#include "mailcommon/mailutil.h"
#include "mailcommon_export.h"

#include <Collection>
#include <EntityTreeView>

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

    Q_REQUIRED_RESULT Akonadi::Collection currentFolder() const;

    void disableSaveConfig();
    void readConfig();

    void updatePalette();

    void keyboardSearch(const QString &) override;

protected:
    enum Move { Next = 0, Previous = 1 };

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
    enum SearchDirection { ForwardSearch, BackwardSearch };

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
