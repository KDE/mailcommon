/*
  SPDX-FileCopyrightText: 2005 Till Adam <adam@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mailutil.h"
#include "mailutil_p.h"

#include "calendarinterface.h"
#include "filter/dialog/filteractionmissingfolderdialog.h"
#include "folder/foldersettings.h"
#include "job/expirejob.h"
#include "kernel/mailkernel.h"
#include "mailcommon_debug.h"
#include "pop3settings.h"

#include <MailImporter/FilterBalsa>
#include <MailImporter/FilterClawsMail>
#include <MailImporter/FilterEvolution>
#include <MailImporter/FilterEvolution_v2>
#include <MailImporter/FilterEvolution_v3>
#include <MailImporter/FilterIcedove>
#include <MailImporter/FilterOpera>
#include <MailImporter/FilterSylpheed>
#include <MailImporter/FilterThunderbird>
#include <MailImporter/OtherMailerUtil>

#include <MessageCore/StringUtil>

#include <MessageComposer/MessageHelper>

#include <AgentManager>
#include <Akonadi/KMime/MessageParts>
#include <Akonadi/KMime/NewMailNotifierAttribute>
#include <AkonadiCore/ServerManager>
#include <EntityTreeModel>
#include <ItemFetchJob>
#include <ItemFetchScope>
#include <entitymimetypefiltermodel.h>

#include <KMime/KMimeMessage>

#include <KColorScheme>
#include <KIO/JobUiDelegate>
#include <KJob>
#include <collectionpage/attributes/expirecollectionattribute.h>

OrgKdeAkonadiPOP3SettingsInterface *MailCommon::Util::createPop3SettingsInterface(const QString &ident)
{
    const auto service = Akonadi::ServerManager::agentServiceName(Akonadi::ServerManager::Resource, ident);
    return new OrgKdeAkonadiPOP3SettingsInterface(service, QStringLiteral("/Settings"), QDBusConnection::sessionBus());
}

bool MailCommon::Util::isVirtualCollection(const Akonadi::Collection &collection)
{
    return collection.isVirtual() || MailCommon::Util::isVirtualCollection(collection.resource());
}

bool MailCommon::Util::isVirtualCollection(const QString &resource)
{
    if (resource == QLatin1String("akonadi_search_resource")) {
        return true;
    }

    const auto type = Akonadi::AgentManager::self()->type(resource);
    return type.capabilities().contains(QLatin1String("Virtual"));
}

bool MailCommon::Util::isLocalCollection(const QString &resource)
{
    auto type = Akonadi::AgentManager::self()->type(resource);
    return type.customProperties().value(QStringLiteral("HasLocalStorage"), false).toBool();
}

QString MailCommon::Util::fullCollectionPath(const Akonadi::Collection &collection, bool addAccountName)
{
    QString fullPath;

    QModelIndex idx = Akonadi::EntityTreeModel::modelIndexForCollection(KernelIf->collectionModel(), collection);
    if (!idx.isValid()) {
        return fullPath;
    }

    fullPath = idx.data().toString();
    idx = idx.parent();
    while (idx != QModelIndex()) {
        const QString tmp = idx.data().toString() + QLatin1Char('/') + fullPath;
        idx = idx.parent();
        if (idx != QModelIndex()) {
            fullPath = tmp;
        } else {
            if (!addAccountName) {
                break;
            } else {
                fullPath = tmp;
                break;
            }
        }
    }
    return fullPath;
}

bool MailCommon::Util::showJobErrorMessage(KJob *job)
{
    if (job->error()) {
        if (auto uidelegate = static_cast<KIO::Job *>(job)->uiDelegate()) {
            uidelegate->showErrorMessage();
        } else {
            qCDebug(MAILCOMMON_LOG) << " job->errorString() :" << job->errorString();
        }
        return true;
    }
    return false;
}

Akonadi::AgentInstance::List MailCommon::Util::agentInstances(bool excludeMailDispacher)
{
    Akonadi::AgentInstance::List relevantInstances;
    const Akonadi::AgentInstance::List agentList = Akonadi::AgentManager::self()->instances();
    std::copy_if(agentList.cbegin(), agentList.cend(), std::back_inserter(relevantInstances), [excludeMailDispacher](const Akonadi::AgentInstance &instance) {
        return isMailAgent(instance, excludeMailDispacher);
    });
    return relevantInstances;
}

bool MailCommon::Util::isMailAgent(const Akonadi::AgentInstance &instance, bool excludeMailTransport)
{
    if (!instance.type().mimeTypes().contains(KMime::Message::mimeType())) {
        return false;
    }

    const QStringList capabilities(instance.type().capabilities());
    if (capabilities.contains(QLatin1String("Resource")) && !capabilities.contains(QLatin1String("Virtual"))
        && !capabilities.contains(QLatin1String("MailTransport")) && !capabilities.contains(QLatin1String("Autostart"))) {
        return true;
    } else if (!excludeMailTransport && instance.identifier() == QLatin1String("akonadi_maildispatcher_agent")) {
        return true;
    }

    return false;
}

bool MailCommon::Util::isUnifiedMailboxesAgent(const Akonadi::Collection &col)
{
    return col.resource() == QLatin1String("akonadi_unifiedmailbox_agent");
}

uint MailCommon::Util::folderIdentity(const Akonadi::Item &item)
{
    uint id = 0;
    if (item.isValid() && item.parentCollection().isValid()) {
        Akonadi::Collection col = item.parentCollection();
        if (col.resource().isEmpty()) {
            col = CommonKernel->collectionFromId(col.id());
        }
        const QSharedPointer<FolderSettings> fd = FolderSettings::forCollection(col, false);

        id = fd->identity();
    }
    return id;
}

bool MailCommon::Util::ignoreNewMailInFolder(const Akonadi::Collection &collection)
{
    if (collection.hasAttribute<Akonadi::NewMailNotifierAttribute>()) {
        if (collection.attribute<Akonadi::NewMailNotifierAttribute>()->ignoreNewMail()) {
            return true;
        }
    }
    return false;
}

QString MailCommon::Util::realFolderPath(const QString &path)
{
    QString realPath(path);
    realPath.remove(QStringLiteral(".directory"));
    realPath.replace(QLatin1String("/."), QStringLiteral("/"));
    if (!realPath.isEmpty() && (realPath.at(0) == QLatin1Char('.'))) {
        realPath.remove(0, 1); // remove first "."
    }
    return realPath;
}

void MailCommon::Util::expireOldMessages(const Akonadi::Collection &collection, bool immediate)
{
    auto task = new ScheduledExpireTask(collection, immediate);
    KernelIf->jobScheduler()->registerTask(task);
}

Akonadi::Collection::Id MailCommon::Util::convertFolderPathToCollectionId(const QString &folder)
{
    Akonadi::Collection::Id newFolderId = -1;
    bool exactPath = false;
    Akonadi::Collection::List lst = FilterActionMissingFolderDialog::potentialCorrectFolders(folder, exactPath);
    if (lst.count() == 1 && exactPath) {
        newFolderId = lst.at(0).id();
    } else {
        QPointer<FilterActionMissingFolderDialog> dlg = new FilterActionMissingFolderDialog(lst, QString(), folder);
        if (dlg->exec()) {
            newFolderId = dlg->selectedCollection().id();
        }
        delete dlg;
    }
    return newFolderId;
}

QString MailCommon::Util::convertFolderPathToCollectionStr(const QString &folder)
{
    Akonadi::Collection::Id newFolderId = MailCommon::Util::convertFolderPathToCollectionId(folder);
    if (newFolderId == -1) {
        return QString();
    }
    return QString::number(newFolderId);
}

void MailCommon::Util::foundMailer(QStringList &lst, const QString &name)
{
    if (!name.isEmpty()) {
        lst.append(name);
    }
}

QStringList MailCommon::Util::foundMailer()
{
    QStringList lst;
    MailCommon::Util::foundMailer(lst, MailImporter::FilterEvolution::isMailerFound());
    MailCommon::Util::foundMailer(lst, MailImporter::FilterEvolution_v2::isMailerFound());
    MailCommon::Util::foundMailer(lst, MailImporter::FilterEvolution_v3::isMailerFound());
    MailCommon::Util::foundMailer(lst, MailImporter::FilterBalsa::isMailerFound());
    MailCommon::Util::foundMailer(lst, MailImporter::FilterClawsMail::isMailerFound());
    MailCommon::Util::foundMailer(lst, MailImporter::FilterOpera::isMailerFound());
    MailCommon::Util::foundMailer(lst, MailImporter::FilterSylpheed::isMailerFound());
    MailCommon::Util::foundMailer(lst, MailImporter::FilterThunderbird::isMailerFound());
    MailCommon::Util::foundMailer(lst, MailImporter::FilterIcedove::isMailerFound());
    const QStringList otherMailer = MailImporter::OtherMailerUtil::isMailerFound();
    if (!otherMailer.isEmpty()) {
        lst << otherMailer;
    }
    return lst;
}
