/*

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FOLDERTREEWIDGET_H
#define MAILCOMMON_FOLDERTREEWIDGET_H

#include "foldertreewidgetproxymodel.h"
#include "mailcommon_export.h"

#include <Collection>

#include <QAbstractItemView>
#include <QWidget>

namespace Akonadi
{
class StatisticsProxyModel;
}

class QLineEdit;
class KXMLGUIClient;

class QItemSelectionModel;

namespace MailCommon
{
class EntityCollectionOrderProxyModel;
class FolderTreeView;

/**
 * This is the widget that shows the main folder tree.
 *
 * It consists of the view (FolderTreeView) and a search line.
 * Internally, several proxy models are used on top of a entity tree model.
 */
class MAILCOMMON_EXPORT FolderTreeWidget : public QWidget
{
    Q_OBJECT

public:
    enum TreeViewOption {
        None = 0,
        ShowUnreadCount = 1,
        UseLineEditForFiltering = 2,
        UseDistinctSelectionModel = 4,
        ShowCollectionStatisticAnimation = 8,
        DontKeyFilter = 16,
        HideStatistics = 32,
        HideHeaderViewMenu = 64
    };
    Q_DECLARE_FLAGS(TreeViewOptions, TreeViewOption)

    explicit FolderTreeWidget(QWidget *parent = nullptr,
                              KXMLGUIClient *xmlGuiClient = nullptr,
                              TreeViewOptions options = (TreeViewOptions)(ShowUnreadCount | ShowCollectionStatisticAnimation),
                              FolderTreeWidgetProxyModel::FolderTreeWidgetProxyModelOptions optReadableProxy = FolderTreeWidgetProxyModel::None);
    ~FolderTreeWidget() override;

    /**
     * The possible tooltip display policies.
     */
    enum ToolTipDisplayPolicy {
        DisplayAlways, ///< Always display a tooltip when hovering over an item
        DisplayWhenTextElided, ///< Display the tooltip if the item text is actually elided
        DisplayNever ///< Nevery display tooltips
    };

    /**
     * The available sorting policies.
     */
    enum SortingPolicy {
        SortByCurrentColumn, ///< Columns are clickable, sorting is by the current column
        SortByDragAndDropKey ///< Columns are NOT clickable, sorting is done by drag and drop
    };

    void selectCollectionFolder(const Akonadi::Collection &col);

    void setSelectionMode(QAbstractItemView::SelectionMode mode);

    Q_REQUIRED_RESULT QAbstractItemView::SelectionMode selectionMode() const;

    Q_REQUIRED_RESULT QItemSelectionModel *selectionModel() const;

    Q_REQUIRED_RESULT QModelIndex currentIndex() const;

    Q_REQUIRED_RESULT Akonadi::Collection selectedCollection() const;

    Q_REQUIRED_RESULT Akonadi::Collection::List selectedCollections() const;

    Q_REQUIRED_RESULT FolderTreeView *folderTreeView() const;

    Q_REQUIRED_RESULT Akonadi::StatisticsProxyModel *statisticsProxyModel() const;

    Q_REQUIRED_RESULT FolderTreeWidgetProxyModel *folderTreeWidgetProxyModel() const;

    Q_REQUIRED_RESULT EntityCollectionOrderProxyModel *entityOrderProxy() const;

    Q_REQUIRED_RESULT QLineEdit *filterFolderLineEdit() const;
    void applyFilter(const QString &);
    void clearFilter();

    void disableContextMenuAndExtraColumn();

    void readConfig();

    void restoreHeaderState(const QByteArray &data);

protected:
    void changeToolTipsPolicyConfig(ToolTipDisplayPolicy);

protected Q_SLOTS:
    void slotChangeTooltipsPolicy(FolderTreeWidget::ToolTipDisplayPolicy);
    void slotManualSortingChanged(bool);
    void slotFilterFixedString(const QString &);
    void slotGeneralFontChanged();
    void slotGeneralPaletteChanged();

private:
    bool eventFilter(QObject *o, QEvent *e) override;
    class FolderTreeWidgetPrivate;
    FolderTreeWidgetPrivate *const d;
};
}

#endif
