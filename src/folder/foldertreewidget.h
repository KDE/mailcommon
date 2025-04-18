/*

  SPDX-FileCopyrightText: 2009-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "foldertreewidgetproxymodel.h"
#include "mailcommon_export.h"

#include <Akonadi/Collection>

#include <QAbstractItemView>
#include <QWidget>
#include <memory>
namespace Akonadi
{
class AccountActivitiesAbstract;
}
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

    void selectCollectionFolder(const Akonadi::Collection &col, bool expand = true);

    void setSelectionMode(QAbstractItemView::SelectionMode mode);

    [[nodiscard]] QAbstractItemView::SelectionMode selectionMode() const;

    [[nodiscard]] QItemSelectionModel *selectionModel() const;

    [[nodiscard]] QModelIndex currentIndex() const;

    [[nodiscard]] Akonadi::Collection selectedCollection() const;

    [[nodiscard]] Akonadi::Collection::List selectedCollections() const;

    [[nodiscard]] FolderTreeView *folderTreeView() const;

    [[nodiscard]] Akonadi::StatisticsProxyModel *statisticsProxyModel() const;

    [[nodiscard]] FolderTreeWidgetProxyModel *folderTreeWidgetProxyModel() const;

    [[nodiscard]] EntityCollectionOrderProxyModel *entityOrderProxy() const;

    [[nodiscard]] QLineEdit *filterFolderLineEdit() const;
    void applyFilter(const QString &);
    void clearFilter();

    void disableContextMenuAndExtraColumn();

    void readConfig();

    void restoreHeaderState(const QByteArray &data);

    void setAccountActivities(Akonadi::AccountActivitiesAbstract *accountActivities);

protected:
    void changeToolTipsPolicyConfig(ToolTipDisplayPolicy);

    [[nodiscard]] bool event(QEvent *e) override;

protected Q_SLOTS:
    void slotChangeTooltipsPolicy(FolderTreeWidget::ToolTipDisplayPolicy);
    void slotManualSortingChanged(bool);
    void slotFilterFixedString(const QString &);
    void slotGeneralFontChanged();
    void slotGeneralPaletteChanged();

private:
    bool eventFilter(QObject *o, QEvent *e) override;
    class FolderTreeWidgetPrivate;
    std::unique_ptr<FolderTreeWidgetPrivate> const d;
};
}
