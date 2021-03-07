/*
  SPDX-FileCopyrightText: 2009 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef MAILCOMMON_BACKUPJOB_H
#define MAILCOMMON_BACKUPJOB_H

#include "mailcommon_export.h"

#include <Libkdepim/ProgressManager>

#include <Collection>
#include <Item>

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
/**
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
    enum ArchiveType { Zip = 0, Tar = 1, TarBz2 = 2, TarGz = 3 };

    explicit BackupJob(QWidget *parent = nullptr);
    ~BackupJob() override;

    void setRootFolder(const Akonadi::Collection &rootFolder);
    void setSaveLocation(const QUrl &savePath);
    void setArchiveType(ArchiveType type);
    void setDeleteFoldersAfterCompletion(bool deleteThem);
    void setRecursive(bool recursive);
    void setDisplayMessageBox(bool display);
    void setRealPath(const QString &path);

    void start();
Q_SIGNALS:
    void backupDone(const QString &);
    void error(const QString &);

private:
    void itemFetchJobResult(KJob *job);
    void cancelJob();
    void archiveNextFolder();
    void onArchiveNextFolderDone(KJob *job);
    void archiveNextMessage();
    bool queueFolders(const Akonadi::Collection &root);
    void processMessage(const Akonadi::Item &item);
    QString pathForCollection(const Akonadi::Collection &collection) const;
    QString subdirPathForCollection(const Akonadi::Collection &collection) const;
    bool hasChildren(const Akonadi::Collection &collection) const;
    void finish();
    void abort(const QString &errorMessage);
    bool writeDirHelper(const QString &directoryPath);

    // Helper function to return the name of the given collection.
    // Some Collection's don't have the name fetched. However, in mAllFolders,
    // we have a list of Collection's that have that information in them, so
    // we can just look it up there.
    QString collectionName(const Akonadi::Collection &collection) const;

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

#endif
