/*
  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "foldercollectionmonitor.h"
#include "collectionpage/attributes/expirecollectionattribute.h"
#include "foldersettings.h"
#include "mailcommon_debug.h"
#include "util/mailutil.h"
#include <Akonadi/KMime/MessageParts>
#include <AkonadiCore/entityannotationsattribute.h>
#include <ChangeRecorder>
#include <Collection>
#include <CollectionFetchScope>
#include <EntityTreeModel>
#include <Item>
#include <ItemDeleteJob>
#include <ItemFetchJob>
#include <ItemFetchScope>

#include <KMime/KMimeMessage>

using namespace MailCommon;

class MailCommon::FolderCollectionMonitorPrivate
{
public:
    Akonadi::ChangeRecorder *mMonitor = nullptr;
};

FolderCollectionMonitor::FolderCollectionMonitor(Akonadi::Session *session, QObject *parent)
    : QObject(parent)
    , d(new MailCommon::FolderCollectionMonitorPrivate)
{
    // monitor collection changes
    d->mMonitor = new Akonadi::ChangeRecorder(this);
    d->mMonitor->setSession(session);
    d->mMonitor->setCollectionMonitored(Akonadi::Collection::root());
    d->mMonitor->fetchCollectionStatistics(true);
    d->mMonitor->collectionFetchScope().setIncludeStatistics(true);
    d->mMonitor->fetchCollection(true);
    d->mMonitor->setAllMonitored(true);
    d->mMonitor->setMimeTypeMonitored(KMime::Message::mimeType());
    d->mMonitor->setResourceMonitored("akonadi_search_resource", true);
    d->mMonitor->itemFetchScope().fetchPayloadPart(Akonadi::MessagePart::Envelope);
    d->mMonitor->itemFetchScope().setFetchModificationTime(false);
    d->mMonitor->itemFetchScope().setFetchRemoteIdentification(false);
    d->mMonitor->itemFetchScope().setFetchTags(true);
    d->mMonitor->itemFetchScope().fetchAttribute<Akonadi::EntityAnnotationsAttribute>(true);
}

FolderCollectionMonitor::~FolderCollectionMonitor()
{
    qDebug() << "FolderCollectionMonitor::~FolderCollectionMonitor()  " << this;
    delete d;
}

Akonadi::ChangeRecorder *FolderCollectionMonitor::monitor() const
{
    return d->mMonitor;
}

void FolderCollectionMonitor::expireAllFolders(bool immediate, QAbstractItemModel *collectionModel)
{
    if (collectionModel) {
        expireAllCollection(collectionModel, immediate);
    }
}

void FolderCollectionMonitor::expireAllCollection(const QAbstractItemModel *model, bool immediate, const QModelIndex &parentIndex)
{
    const int rowCount = model->rowCount(parentIndex);
    for (int row = 0; row < rowCount; ++row) {
        const QModelIndex index = model->index(row, 0, parentIndex);
        const auto collection = model->data(index, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();

        if (!collection.isValid() || Util::isVirtualCollection(collection)) {
            continue;
        }

        const auto *attr = collection.attribute<MailCommon::ExpireCollectionAttribute>();
        if (attr) {
            if (attr->isAutoExpire()) {
                MailCommon::Util::expireOldMessages(collection, immediate);
            }
        }

        if (model->rowCount(index) > 0) {
            expireAllCollection(model, immediate, index);
        }
    }
}

void FolderCollectionMonitor::expunge(const Akonadi::Collection &col, bool sync)
{
    if (col.isValid()) {
        auto job = new Akonadi::ItemDeleteJob(col, this);
        connect(job, &Akonadi::ItemDeleteJob::result, this, &FolderCollectionMonitor::slotDeleteJob);
        if (sync) {
            job->exec();
        }
    } else {
        qCDebug(MAILCOMMON_LOG) << " Try to expunge an invalid collection :" << col;
    }
}

void FolderCollectionMonitor::slotDeleteJob(KJob *job)
{
    Util::showJobErrorMessage(job);
}
