/*
  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FOLDERCOLLECTIONMONITOR_H
#define MAILCOMMON_FOLDERCOLLECTIONMONITOR_H

#include "mailcommon_export.h"

#include <KIO/Job>
#include <KSharedConfig>

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
/**
 * @brief The FolderCollectionMonitor class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FolderCollectionMonitor : public QObject
{
    Q_OBJECT

public:
    explicit FolderCollectionMonitor(Akonadi::Session *session, QObject *parent = nullptr);
    ~FolderCollectionMonitor() override;

    Q_REQUIRED_RESULT Akonadi::ChangeRecorder *monitor() const;
    void expireAllFolders(bool immediate, QAbstractItemModel *collectionModel);
    void expunge(const Akonadi::Collection &, bool sync = false);

protected:
    void expireAllCollection(const QAbstractItemModel *model, bool immediate, const QModelIndex &parentIndex = QModelIndex());

private:
    void slotDeleteJob(KJob *job);
    FolderCollectionMonitorPrivate *const d;
};
}

#endif
