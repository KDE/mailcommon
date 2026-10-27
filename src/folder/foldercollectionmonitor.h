/*
  SPDX-FileCopyrightText: 2009-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"

#include <KIO/Job>

#include <QModelIndex>
#include <QObject>

class QAbstractItemModel;

namespace Akonadi
{
class ChangeRecorder;
class Collection;
class Session;
}

namespace MailCommon
{
class FolderCollectionMonitorPrivate;
/*!
 * \class MailCommon::FolderCollectionMonitor
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FolderCollectionMonitor
 *
 * \brief The FolderCollectionMonitor class.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FolderCollectionMonitor : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs a folder collection monitor using the Akonadi \a session, with the given \a parent.
     */
    explicit FolderCollectionMonitor(Akonadi::Session *session, QObject *parent = nullptr);
    /*!
     * Destroys the folder collection monitor.
     */
    ~FolderCollectionMonitor() override;
    /*!
     * Returns the change recorder used to monitor the collections.
     */
    [[nodiscard]] Akonadi::ChangeRecorder *monitor() const;
    /*!
     * Expires the old messages of all the collections of \a collectionModel that have auto-expiry enabled.
     * If \a immediate is \c true, the expiry is done immediately.
     */
    void expireAllFolders(bool immediate, QAbstractItemModel *collectionModel);
    /*!
     * Deletes all the items of the collection \a collection.
     * If \a sync is \c true, the deletion is done synchronously.
     */
    void expunge(const Akonadi::Collection &collection, bool sync = false);

protected:
    /*!
     * Expires the old messages of the collections of \a model under \a parentIndex, recursively.
     * If \a immediate is \c true, the expiry is done immediately.
     */
    void expireAllCollection(const QAbstractItemModel *model, bool immediate, const QModelIndex &parentIndex = QModelIndex());

private:
    MAILCOMMON_NO_EXPORT void slotDeleteJob(KJob *job);
    std::unique_ptr<FolderCollectionMonitorPrivate> const d;
};
}
