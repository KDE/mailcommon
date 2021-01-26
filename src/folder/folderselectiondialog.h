/*

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FOLDERSELECTIONDIALOG_H
#define MAILCOMMON_FOLDERSELECTIONDIALOG_H

#include "mailcommon_export.h"

#include <Collection>

#include <QDialog>

#include <QAbstractItemView>

class KJob;

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
    Q_REQUIRED_RESULT QAbstractItemView::SelectionMode selectionMode() const;

    Q_REQUIRED_RESULT Akonadi::Collection selectedCollection() const;
    void setSelectedCollection(const Akonadi::Collection &collection);

    Q_REQUIRED_RESULT Akonadi::Collection::List selectedCollections() const;

private Q_SLOTS:
    void slotSelectionChanged();
    void slotAddChildFolder();
    void collectionCreationResult(KJob *);
    void rowsInserted(const QModelIndex &col, int, int);
    void slotDoubleClick(const QModelIndex &);
    void slotFolderTreeWidgetContextMenuRequested(const QPoint &);

protected:
    void focusTreeView();
    void readConfig();
    void writeConfig();
    bool canCreateCollection(Akonadi::Collection &parentCol);

    void hideEvent(QHideEvent *) override;

    void showEvent(QShowEvent *) override;

private:
    class FolderSelectionDialogPrivate;
    FolderSelectionDialogPrivate *const d;
};
}

#endif
