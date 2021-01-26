/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filtertestkernel.h"

#include <AkonadiCore/changerecorder.h>
#include <AkonadiCore/entitymimetypefiltermodel.h>
#include <AkonadiCore/entitytreemodel.h>
#include <AkonadiCore/session.h>
#include <KIdentityManagement/kidentitymanagement/identitymanager.h>
#include <KSharedConfig>
#include <MessageComposer/AkonadiSender>
#include <mailcommon/foldercollectionmonitor.h>

FilterTestKernel::FilterTestKernel(QObject *parent)
    : QObject(parent)
{
    mMessageSender = new MessageComposer::AkonadiSender(this);
    mIdentityManager = new KIdentityManagement::IdentityManager(true, this);
    auto session = new Akonadi::Session("Filter Kernel ETM", this);
    mFolderCollectionMonitor = new MailCommon::FolderCollectionMonitor(session, this);

    mEntityTreeModel = new Akonadi::EntityTreeModel(folderCollectionMonitor(), this);
    mEntityTreeModel->setListFilter(Akonadi::CollectionFetchScope::Enabled);
    mEntityTreeModel->setItemPopulationStrategy(Akonadi::EntityTreeModel::LazyPopulation);

    mCollectionModel = new Akonadi::EntityMimeTypeFilterModel(this);
    mCollectionModel->setSourceModel(mEntityTreeModel);
    mCollectionModel->addMimeTypeInclusionFilter(Akonadi::Collection::mimeType());
    mCollectionModel->setHeaderGroup(Akonadi::EntityTreeModel::CollectionTreeHeaders);
    mCollectionModel->setDynamicSortFilter(true);
    mCollectionModel->setSortCaseSensitivity(Qt::CaseInsensitive);
}

KIdentityManagement::IdentityManager *FilterTestKernel::identityManager()
{
    return mIdentityManager;
}

MessageComposer::MessageSender *FilterTestKernel::msgSender()
{
    return mMessageSender;
}

Akonadi::EntityMimeTypeFilterModel *FilterTestKernel::collectionModel() const
{
    return mCollectionModel;
}

KSharedConfig::Ptr FilterTestKernel::config()
{
    return KSharedConfig::openConfig();
}

void FilterTestKernel::syncConfig()
{
    Q_ASSERT(false);
}

MailCommon::JobScheduler *FilterTestKernel::jobScheduler() const
{
    Q_ASSERT(false);
    return nullptr;
}

Akonadi::ChangeRecorder *FilterTestKernel::folderCollectionMonitor() const
{
    return mFolderCollectionMonitor->monitor();
}

void FilterTestKernel::updateSystemTray()
{
    Q_ASSERT(false);
}

bool FilterTestKernel::showPopupAfterDnD()
{
    return false;
}

void FilterTestKernel::expunge(Akonadi::Collection::Id col, bool sync)
{
    Q_UNUSED(col)
    Q_UNUSED(sync)
}

qreal FilterTestKernel::closeToQuotaThreshold()
{
    return 80;
}

QStringList FilterTestKernel::customTemplates()
{
    Q_ASSERT(false);
    return QStringList();
}

bool FilterTestKernel::excludeImportantMailFromExpiry()
{
    Q_ASSERT(false);
    return true;
}

Akonadi::Collection::Id FilterTestKernel::lastSelectedFolder()
{
    Q_ASSERT(false);
    return Akonadi::Collection::Id();
}

void FilterTestKernel::setLastSelectedFolder(Akonadi::Collection::Id col)
{
    Q_UNUSED(col)
}
