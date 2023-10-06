/*
  SPDX-FileCopyrightText: 2009-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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

    [[nodiscard]] Akonadi::ChangeRecorder *monitor() const;
    void expireAllFolders(bool immediate, QAbstractItemModel *collectionModel);
    void expunge(const Akonadi::Collection &, bool sync = false);

protected:
    void expireAllCollection(const QAbstractItemModel *model, bool immediate, const QModelIndex &parentIndex = QModelIndex());

private:
    MAILCOMMON_NO_EXPORT void slotDeleteJob(KJob *job);
    std::unique_ptr<FolderCollectionMonitorPrivate> const d;
};
}
