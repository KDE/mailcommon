/*
  SPDX-FileCopyrightText: 2009 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once

#include "mailcommon_export.h"

#include <Libkdepim/ProgressManager>

#include <Akonadi/Collection>
#include <Akonadi/Item>

#include <QUrl>

#include <QDateTime>
#include <QObject>
#include <QPointer>

class KArchive;
class KJob;

class QWidget;

namespace Akonadi
{
class ItemFetchJob;
}

namespace MailCommon
{
/*!
 * \class MailCommon::BackupJob
 * \inmodule MailCommon
 * \inheaderfile MailCommon/BackupJob
 *
 * Writes an entire folder structure to an archive file. The archive is
 * structured like a hierarchy of maildir folders. However, every type of
 * folder works as the source, i.e. also online IMAP folders.
 *
 * The job deletes itself after it finished.
 */
class MAILCOMMON_EXPORT BackupJob : public QObject
{
    Q_OBJECT

public:
    // These enum values have to stay in sync with the format combobox of ArchiveFolderDialog!
    enum ArchiveType {
        Zip = 0,
        Tar = 1,
        TarBz2 = 2,
        TarGz = 3
    };

    /*!
     * Constructs a backup job.
     *
     * \param parent The parent widget
     */
    explicit BackupJob(QWidget *parent = nullptr);
    /*!
     * Destroys the backup job.
     */
    ~BackupJob() override;

    /*!
     * Sets the root folder to backup.
     *
     * \param rootFolder The root collection to backup
     */
    void setRootFolder(const Akonadi::Collection &rootFolder);
    /*!
     * Sets the location to save the backup archive.
     *
     * \param savePath The path where the archive should be saved
     */
    void setSaveLocation(const QUrl &savePath);
    /*!
     * Sets the type of archive to create.
     *
     * \param type The archive type (Zip, Tar, TarBz2, or TarGz)
     */
    void setArchiveType(ArchiveType type);
    /*!
     * Sets whether to delete folders after the backup is complete.
     *
     * \param deleteThem Whether to delete folders after backup
     */
    void setDeleteFoldersAfterCompletion(bool deleteThem);
    /*!
     * Sets whether to backup recursively (all subfolders).
     *
     * \param recursive Whether to backup recursively
     */
    void setRecursive(bool recursive);
    /*!
     * Sets whether to display a message box when the backup is complete.
     *
     * \param display Whether to display completion message
     */
    void setDisplayMessageBox(bool display);
    /*!
     * Sets the real path of the folder to backup.
     *
     * \param path The real path to backup
     */
    void setRealPath(const QString &path);

    /*!
     * Starts the backup job.
     */
    void start();
Q_SIGNALS:
    /*!
     * Emitted when the backup has completed successfully.
     *
     * \param message A message describing the backup completion
     */
    void backupDone(const QString &message);
    /*!
     * Emitted when an error occurs during the backup.
     *
     * \param message A message describing the error
     */
    void error(const QString &message);

private:
    MAILCOMMON_NO_EXPORT void itemFetchJobResult(KJob *job);
    MAILCOMMON_NO_EXPORT void cancelJob();
    MAILCOMMON_NO_EXPORT void archiveNextFolder();
    MAILCOMMON_NO_EXPORT void onArchiveNextFolderDone(KJob *job);
    MAILCOMMON_NO_EXPORT void archiveNextMessage();
    [[nodiscard]] MAILCOMMON_NO_EXPORT bool queueFolders(const Akonadi::Collection &root);
    MAILCOMMON_NO_EXPORT void processMessage(const Akonadi::Item &item);
    [[nodiscard]] MAILCOMMON_NO_EXPORT QString pathForCollection(const Akonadi::Collection &collection) const;
    [[nodiscard]] MAILCOMMON_NO_EXPORT QString subdirPathForCollection(const Akonadi::Collection &collection) const;
    [[nodiscard]] MAILCOMMON_NO_EXPORT bool hasChildren(const Akonadi::Collection &collection) const;
    MAILCOMMON_NO_EXPORT void finish();
    MAILCOMMON_NO_EXPORT void abort(const QString &errorMessage);
    [[nodiscard]] MAILCOMMON_NO_EXPORT bool writeDirHelper(const QString &directoryPath);

    // Helper function to return the name of the given collection.
    // Some Collection's don't have the name fetched. However, in mAllFolders,
    // we have a list of Collection's that have that information in them, so
    // we can just look it up there.
    [[nodiscard]] MAILCOMMON_NO_EXPORT QString collectionName(const Akonadi::Collection &collection) const;

    QString mRealPath;
    QUrl mMailArchivePath;
    QDateTime mArchiveTime;
    ArchiveType mArchiveType = Zip;
    Akonadi::Collection mRootFolder;
    KArchive *mArchive = nullptr;
    QWidget *const mParentWidget;
    int mArchivedMessages = 0;
    uint mArchivedSize = 0;
    QPointer<KPIM::ProgressItem> mProgressItem;
    bool mAborted = false;
    bool mDeleteFoldersAfterCompletion = false;
    bool mRecursive = true;

    Akonadi::Collection::List mPendingFolders;
    Akonadi::Collection::List mAllFolders;
    Akonadi::Collection mCurrentFolder;
    Akonadi::Item::List mPendingMessages;
    Akonadi::ItemFetchJob *mCurrentJob = nullptr;
    bool mDisplayMessageBox = true;
};
}
