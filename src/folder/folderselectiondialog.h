/*

  SPDX-FileCopyrightText: 2009-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"

#include <Akonadi/Collection>

#include <QDialog>

#include <QAbstractItemView>

class KJob;
namespace Akonadi
{
class AccountActivitiesAbstract;
}
namespace MailCommon
{
/**
 * A dialog that lets the user select a folder.
 * TODO: Move most of this to Akonadi::CollectionDialog
 */
class MAILCOMMON_EXPORT FolderSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    enum SelectionFolderOption {
        None = 0,
        EnableCheck = 1,
        ShowUnreadCount = 2,
        HideVirtualFolder = 4,
        NotAllowToCreateNewFolder = 8,
        HideOutboxFolder = 16,
        NotUseGlobalSettings = 64
    };
    Q_DECLARE_FLAGS(SelectionFolderOptions, SelectionFolderOption)

    FolderSelectionDialog(QWidget *parent, FolderSelectionDialog::SelectionFolderOptions options);
    ~FolderSelectionDialog() override;

    void setSelectionMode(QAbstractItemView::SelectionMode mode);
    [[nodiscard]] QAbstractItemView::SelectionMode selectionMode() const;

    [[nodiscard]] Akonadi::Collection selectedCollection() const;
    void setSelectedCollection(const Akonadi::Collection &collection);

    [[nodiscard]] Akonadi::Collection::List selectedCollections() const;

    void setAccountActivities(Akonadi::AccountActivitiesAbstract *accountActivities);

private Q_SLOTS:
    MAILCOMMON_NO_EXPORT void slotSelectionChanged();
    MAILCOMMON_NO_EXPORT void slotAddChildFolder();
    MAILCOMMON_NO_EXPORT void collectionCreationResult(KJob *);
    MAILCOMMON_NO_EXPORT void rowsInserted(const QModelIndex &col, int, int);
    MAILCOMMON_NO_EXPORT void slotDoubleClick(const QModelIndex &);
    MAILCOMMON_NO_EXPORT void slotFolderTreeWidgetContextMenuRequested(const QPoint &);

protected:
    void focusTreeView();
    void readConfig();
    void writeConfig();
    bool canCreateCollection(Akonadi::Collection &parentCol);

    void hideEvent(QHideEvent *) override;

    void showEvent(QShowEvent *) override;

private:
    class FolderSelectionDialogPrivate;
    std::unique_ptr<FolderSelectionDialogPrivate> const d;
};
}
