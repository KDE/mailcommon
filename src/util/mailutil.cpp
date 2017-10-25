/*******************************************************************************
**
** Filename   : util
** Created on : 03 April, 2005
** Copyright  : (c) 2005 Till Adam
** Email      : <adam@kde.org>
**
*******************************************************************************/

/*******************************************************************************
**
**   This program is free software; you can redistribute it and/or modify
**   it under the terms of the GNU General Public License as published by
**   the Free Software Foundation; either version 2 of the License, or
**   (at your option) any later version.
**
**   It is distributed in the hope that it will be useful, but
**   WITHOUT ANY WARRANTY; without even the implied warranty of
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**   General Public License for more details.
**
**   You should have received a copy of the GNU General Public License
**   along with this program; if not, write to the Free Software
**   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**   In addition, as a special exception, the copyright holders give
**   permission to link the code of this program with any edition of
**   the Qt library by Trolltech AS, Norway (or with modified versions
**   of Qt that use the same license as Qt), and distribute linked
**   combinations including the two.  You must obey the GNU General
**   Public License in all respects for all of the code used other than
**   Qt.  If you modify this file, you may extend this exception to
**   your version of the file, but you are not obligated to do so.  If
**   you do not wish to do so, delete this exception statement from
**   your version.
**
*******************************************************************************/

#include "mailutil.h"
#include "mailutil_p.h"

#include "mailcommon_debug.h"
#include "calendarinterface.h"
#include "job/expirejob.h"
#include "folder/foldersettings.h"
#include "pop3settings.h"
#include "kernel/mailkernel.h"
#include "filter/dialog/filteractionmissingfolderdialog.h"

#include "mailimporter/filterbalsa.h"
#include "mailimporter/filterevolution.h"
#include "mailimporter/filterevolution_v2.h"
#include "mailimporter/filterevolution_v3.h"
#include "mailimporter/filterclawsmail.h"
#include "mailimporter/filtersylpheed.h"
#include "mailimporter/filterthunderbird.h"
#include "mailimporter/filteropera.h"
#include "mailimporter/filtericedove.h"
#include "mailimporter/othermailerutil.h"

#include <MessageCore/StringUtil>

#include <MessageComposer/MessageHelper>

#include <AgentManager>
#include <entitymimetypefiltermodel.h>
#include <EntityTreeModel>
#include <ItemFetchJob>
#include <ItemFetchScope>
#include <Akonadi/KMime/MessageParts>
#include <AkonadiCore/NewMailNotifierAttribute>
#include <AkonadiCore/ServerManager>

#include <KMime/KMimeMessage>

#include <KColorScheme>
#include <KJob>
#include <KIO/JobUiDelegate>
#include <collectionpage/attributes/expirecollectionattribute.h>

OrgKdeAkonadiPOP3SettingsInterface *MailCommon::Util::createPop3SettingsInterface(
    const QString &ident)
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
    return type.capabilities().contains(QStringLiteral("Virtual"));
}

bool MailCommon::Util::isLocalCollection(const QString &resource)
{
    auto type = Akonadi::AgentManager::self()->type(resource);
    return type.customProperties().value(QStringLiteral("HasLocalStorage"), false).toBool();
}

QString MailCommon::Util::fullCollectionPath(const Akonadi::Collection &collection, bool addAccountName)
{
    QString fullPath;

    QModelIndex idx
        = Akonadi::EntityTreeModel::modelIndexForCollection(KernelIf->collectionModel(), collection);
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
    for (const Akonadi::AgentInstance &instance : agentList) {
        const QStringList capabilities(instance.type().capabilities());
        if (instance.type().mimeTypes().contains(KMime::Message::mimeType())) {
            if (capabilities.contains(QStringLiteral("Resource"))
                && !capabilities.contains(QStringLiteral("Virtual"))
                && !capabilities.contains(QStringLiteral("MailTransport"))) {
                relevantInstances << instance;
            } else if (!excludeMailDispacher
                       && instance.identifier() == QLatin1String("akonadi_maildispatcher_agent")) {
                relevantInstances << instance;
            }
        }
    }
    return relevantInstances;
}

uint MailCommon::Util::folderIdentity(const Akonadi::Item &item)
{
    uint id = 0;
    if (item.isValid() && item.parentCollection().isValid()) {
        Akonadi::Collection col = item.parentCollection();
        if (col.resource().isEmpty()) {
            col = CommonKernel->collectionFromId(col.id());
        }
        const QSharedPointer<FolderSettings> fd
            = FolderSettings::forCollection(col, false);

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
        realPath.remove(0, 1);   //remove first "."
    }
    return realPath;
}

QColor MailCommon::Util::defaultQuotaColor()
{
    KColorScheme scheme(QPalette::Active, KColorScheme::View);
    return scheme.foreground(KColorScheme::NegativeText).color();
}

void MailCommon::Util::expireOldMessages(const Akonadi::Collection &collection, bool immediate)
{
    ScheduledExpireTask *task = new ScheduledExpireTask(collection, immediate);
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

MailCommon::ExpireCollectionAttribute *MailCommon::Util::expirationCollectionAttribute(const Akonadi::Collection &collection, bool &mustDeleteExpirationAttribute)
{
    MailCommon::ExpireCollectionAttribute *attr = nullptr;
    if (collection.hasAttribute<MailCommon::ExpireCollectionAttribute>()) {
        attr = collection.attribute<MailCommon::ExpireCollectionAttribute>();
        mustDeleteExpirationAttribute = false;
    } else {
        attr = new MailCommon::ExpireCollectionAttribute();
        KConfigGroup configGroup(KernelIf->config(),
                                 MailCommon::FolderSettings::configGroupName(collection));

        if (configGroup.hasKey("ExpireMessages")) {
            attr->setAutoExpire(configGroup.readEntry("ExpireMessages", false));
            attr->setReadExpireAge(configGroup.readEntry("ReadExpireAge", 3));
            attr->setReadExpireUnits((MailCommon::ExpireCollectionAttribute::ExpireUnits)configGroup.readEntry("ReadExpireUnits", (int)MailCommon::ExpireCollectionAttribute::ExpireMonths));
            attr->setUnreadExpireAge(configGroup.readEntry("UnreadExpireAge", 12));
            attr->setUnreadExpireUnits((MailCommon::ExpireCollectionAttribute::ExpireUnits)configGroup.readEntry("UnreadExpireUnits", (int)MailCommon::ExpireCollectionAttribute::ExpireNever));
            attr->setExpireAction(configGroup.readEntry("ExpireAction", "Delete") == QLatin1String("Move")
                                  ? MailCommon::ExpireCollectionAttribute::ExpireMove
                                  : MailCommon::ExpireCollectionAttribute::ExpireDelete);
            attr->setExpireToFolderId(configGroup.readEntry("ExpireToFolder", -1));
        }

        mustDeleteExpirationAttribute = true;
    }
    return attr;
}
