/*
 * SPDX-FileCopyrightText: 2004 Carsten Burghardt <burghardt@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#ifndef MAILCOMMON_FOLDERREQUESTER_H
#define MAILCOMMON_FOLDERREQUESTER_H

#include "mailcommon_export.h"

#include <Collection>

#include <QWidget>

class QKeyEvent;

class KJob;

namespace MailCommon
{
/**
 * A widget that contains a QLineEdit which shows the current folder
 * and a button that fires a FolderSelectionDialog
 * The dialog is set to disable readonly folders by default
 * Search folders are excluded
 *
 * @todo This should be cleaned up and go into libakonadi. This includes:
 * - s/Folder/Collection/g
 * - Use Akonadi::CollectionDialog instead of MailCommon::FolderSelectionDialog
 *  - merge that into CollectionDialog
 *  - or allow to replace the built-in dialog by your own
 * - Allow to pass in an existing ETM, to remove the Kernel dependency
 */
class FolderRequesterPrivate;
class MAILCOMMON_EXPORT FolderRequester : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent the parent widget
     */
    explicit FolderRequester(QWidget *parent = nullptr);
    ~FolderRequester() override;

    /**
     * Returns the selected collection.
     */
    Q_REQUIRED_RESULT Akonadi::Collection collection() const;

    /**
     * Presets the folder to the collection @p collection.
     * Disable fetchcollection when not necessary @p fetchCollection
     */
    void setCollection(const Akonadi::Collection &collection, bool fetchCollection = true);

    /**
     * Returns @c true if there's a valid collection set on this widget.
     */
    Q_REQUIRED_RESULT bool hasCollection() const;

    /**
     * Sets if readonly folders should be disabled.
     * Be aware that if you disable this the user can also select the
     * 'Local Folders' folder which has no valid folder associated
     */
    void setMustBeReadWrite(bool readwrite);

    void setShowOutbox(bool show);

    void setNotAllowToCreateNewFolder(bool notCreateNewFolder);

    void setSelectFolderTitleDialog(const QString &title);

protected Q_SLOTS:
    /**
     * Opens the folder dialog.
     */
    void slotOpenDialog();

    /**
     * Updates the information we have about the current folder.
     */
    void slotCollectionsReceived(KJob *);

Q_SIGNALS:
    /**
     * Emitted when the folder changed.
     */
    void folderChanged(const Akonadi::Collection &);
    void invalidFolder();

protected:
    /** Capture space key to open the dialog */
    void keyPressEvent(QKeyEvent *e) override;
    void setCollectionFullPath(const Akonadi::Collection &col);

protected:
    FolderRequesterPrivate *const d;
};
}

#endif
