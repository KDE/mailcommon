/*

  SPDX-FileCopyrightText: 2009-2026 Laurent Montel <montel@kde.org>

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

/*!
 * \class MailCommon::FolderTreeWidget
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FolderTreeWidget
 *
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

    /*!
     * Constructs a folder tree widget with the given \a parent, using \a xmlGuiClient for its actions.
     * The widget is configured with \a options and the folder proxy model with \a optReadableProxy.
     */
    explicit FolderTreeWidget(QWidget *parent = nullptr,
                              KXMLGUIClient *xmlGuiClient = nullptr,
                              TreeViewOptions options = (TreeViewOptions)(ShowUnreadCount | ShowCollectionStatisticAnimation),
                              FolderTreeWidgetProxyModel::FolderTreeWidgetProxyModelOptions optReadableProxy = FolderTreeWidgetProxyModel::None);
    ~FolderTreeWidget() override;

    /*!
     * The possible tooltip display policies.
     *
     * \value DisplayAlways Always display a tooltip when hovering over an item
     * \value DisplayWhenTextElided Display the tooltip if the item text is actually elided
     * \value DisplayNever Never display tooltips
     */
    enum ToolTipDisplayPolicy {
        DisplayAlways,
        DisplayWhenTextElided,
        DisplayNever
    };

    /*!
     * The available sorting policies.
     *
     * \value SortByCurrentColumn Columns are clickable, sorting is by the current column
     * \value SortByDragAndDropKey Columns are NOT clickable, sorting is done by drag and drop
     */
    enum SortingPolicy {
        SortByCurrentColumn,
        SortByDragAndDropKey
    };

    /*!
     * Selects the folder of the collection \a col, expanding it if \a expand is \c true.
     */
    void selectCollectionFolder(const Akonadi::Collection &col, bool expand = true);

    /*!
     * Sets the selection \a mode of the folder tree view.
     */
    void setSelectionMode(QAbstractItemView::SelectionMode mode);

    /*!
     * Returns the selection mode of the folder tree view.
     */
    [[nodiscard]] QAbstractItemView::SelectionMode selectionMode() const;

    /*!
     * Returns the selection model of the folder tree view.
     */
    [[nodiscard]] QItemSelectionModel *selectionModel() const;

    /*!
     * Returns the current index of the folder tree view.
     */
    [[nodiscard]] QModelIndex currentIndex() const;

    /*!
     * Returns the selected collection.
     */
    [[nodiscard]] Akonadi::Collection selectedCollection() const;

    /*!
     * Returns the selected collections.
     */
    [[nodiscard]] Akonadi::Collection::List selectedCollections() const;

    /*!
     * Returns the folder tree view.
     */
    [[nodiscard]] FolderTreeView *folderTreeView() const;

    /*!
     * Returns the statistics proxy model.
     */
    [[nodiscard]] Akonadi::StatisticsProxyModel *statisticsProxyModel() const;

    /*!
     * Returns the folder tree widget proxy model.
     */
    [[nodiscard]] FolderTreeWidgetProxyModel *folderTreeWidgetProxyModel() const;

    /*!
     * Returns the proxy model used to order the collections.
     */
    [[nodiscard]] EntityCollectionOrderProxyModel *entityOrderProxy() const;

    /*!
     * Returns the line edit used to filter the folders.
     */
    [[nodiscard]] QLineEdit *filterFolderLineEdit() const;
    /*!
     * Filters the folders with the given text.
     */
    void applyFilter(const QString &);
    /*!
     * Clears the folder filter.
     */
    void clearFilter();

    /*!
     * Disables the context menu and hides all the columns except the first one.
     */
    void disableContextMenuAndExtraColumn();

    /*!
     * Reads the configuration of the widget.
     */
    void readConfig();

    /*!
     * Restores the header state from \a data.
     */
    void restoreHeaderState(const QByteArray &data);

    /*!
     * Sets \a accountActivities, used to filter the folders by activity.
     */
    void setAccountActivities(Akonadi::AccountActivitiesAbstract *accountActivities);

    /*!
     * Expands all the folders.
     */
    void expandAll();
    /*!
     * Collapses all the folders.
     */
    void collapseAll();

protected:
    /*!
     * Changes the tooltip display policy.
     */
    void changeToolTipsPolicyConfig(ToolTipDisplayPolicy);

    /*!
     * Reimplemented to update the palette when the application palette changes.
     * Returns \c true if the event \a e was recognized.
     */
    [[nodiscard]] bool event(QEvent *e) override;

protected Q_SLOTS:
    /*!
     * Changes the tooltip display policy.
     */
    void slotChangeTooltipsPolicy(MailCommon::FolderTreeWidget::ToolTipDisplayPolicy);
    /*!
     * Enables or disables manual sorting.
     */
    void slotManualSortingChanged(bool);
    /*!
     * Filters the folders with the given text.
     */
    void slotFilterFixedString(const QString &);
    /*!
     * Updates the font when the general font changes.
     */
    void slotGeneralFontChanged();
    /*!
     * Updates the palette when the general palette changes.
     */
    void slotGeneralPaletteChanged();

private:
    bool eventFilter(QObject *o, QEvent *e) override;
    class FolderTreeWidgetPrivate;
    std::unique_ptr<FolderTreeWidgetPrivate> const d;
};
}
