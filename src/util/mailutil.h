/*
  SPDX-FileCopyrightText: 2005 Till Adam <adam@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_MAILUTIL_H
#define MAILCOMMON_MAILUTIL_H

#include "mailcommon_export.h"

#include <AgentInstance>
#include <Collection>

class OrgKdeAkonadiPOP3SettingsInterface;

namespace Akonadi
{
class Item;
}
class KJob;

class QString;

namespace MailCommon
{
/**
 * The Util namespace contains a collection of helper functions use in
 * various places.
 */
namespace Util
{
Q_REQUIRED_RESULT MAILCOMMON_EXPORT OrgKdeAkonadiPOP3SettingsInterface *createPop3SettingsInterface(const QString &ident);

Q_REQUIRED_RESULT MAILCOMMON_EXPORT bool isVirtualCollection(const Akonadi::Collection &col);

Q_REQUIRED_RESULT MAILCOMMON_EXPORT bool isVirtualCollection(const QString &resource);

Q_REQUIRED_RESULT MAILCOMMON_EXPORT QString fullCollectionPath(const Akonadi::Collection &collection, bool addAccountName = true);

MAILCOMMON_EXPORT bool showJobErrorMessage(KJob *job);

Q_REQUIRED_RESULT MAILCOMMON_EXPORT Akonadi::AgentInstance::List agentInstances(bool excludeMailTransport = true);

Q_REQUIRED_RESULT MAILCOMMON_EXPORT bool isMailAgent(const Akonadi::AgentInstance &instance, bool excludeMailTransport = true);

Q_REQUIRED_RESULT MAILCOMMON_EXPORT bool isUnifiedMailboxesAgent(const Akonadi::Collection &col);

/**
 * Returns the identity of the folder that contains the given Akonadi::Item.
 */
Q_REQUIRED_RESULT MAILCOMMON_EXPORT uint folderIdentity(const Akonadi::Item &item);

Q_REQUIRED_RESULT MAILCOMMON_EXPORT QString realFolderPath(const QString &path);

MAILCOMMON_EXPORT void expireOldMessages(const Akonadi::Collection &collection, bool immediate);

Q_REQUIRED_RESULT MAILCOMMON_EXPORT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &folder);
Q_REQUIRED_RESULT MAILCOMMON_EXPORT QString convertFolderPathToCollectionStr(const QString &folder);

MAILCOMMON_EXPORT void foundMailer(QStringList &lst, const QString &name);
Q_REQUIRED_RESULT MAILCOMMON_EXPORT QStringList foundMailer();
Q_REQUIRED_RESULT MAILCOMMON_EXPORT bool isLocalCollection(const QString &resource);
}
}

#endif
