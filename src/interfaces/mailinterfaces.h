/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_MAILINTERFACES_H
#define MAILCOMMON_MAILINTERFACES_H

#include <Collection>

#include <KSharedConfig>

namespace MessageComposer
{
class MessageSender;
}

namespace Akonadi
{
class ChangeRecorder;
class EntityMimeTypeFilterModel;
}

namespace KIdentityManagement
{
class IdentityManager;
}

namespace MailCommon
{
class FilterDialog;
class JobScheduler;

/** Generic interface for mail kernels.*/
class IKernel
{
public:
    /**
     * Returns a model of all folders in KMail.
     * This is basically the same as entityTreeModel(), but with items
     * filtered out, the model contains only collections.
     */
    virtual Akonadi::EntityMimeTypeFilterModel *collectionModel() const = 0;

    /**
     * Return the pointer to the identity manager.
     */
    virtual KIdentityManagement::IdentityManager *identityManager() = 0;

    virtual KSharedConfig::Ptr config() = 0;
    virtual void syncConfig() = 0;
    virtual JobScheduler *jobScheduler() const = 0;
    virtual Akonadi::ChangeRecorder *folderCollectionMonitor() const = 0;
    virtual void updateSystemTray() = 0;
    virtual MessageComposer::MessageSender *msgSender() = 0;
    virtual void expunge(Akonadi::Collection::Id col, bool sync) = 0;

    virtual ~IKernel()
    {
    }
};

/** Filter related interface */
class IFilter
{
public:
    virtual void openFilterDialog(bool createDummyFilter = true) = 0;
    virtual void createFilter(const QByteArray &field, const QString &value) = 0;
    virtual ~IFilter()
    {
    }
};

/** Interface to access some settings. */
class ISettings
{
public:
    virtual bool showPopupAfterDnD() = 0;

    virtual bool excludeImportantMailFromExpiry() = 0;

    virtual qreal closeToQuotaThreshold() = 0;

    virtual Akonadi::Collection::Id lastSelectedFolder() = 0;
    virtual void setLastSelectedFolder(Akonadi::Collection::Id col) = 0;

    virtual QStringList customTemplates() = 0;

    virtual ~ISettings()
    {
    }
};
}

#endif
