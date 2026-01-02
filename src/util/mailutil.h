/*
  SPDX-FileCopyrightText: 2005 Till Adam <adam@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"

#include <Akonadi/AgentInstance>
#include <Akonadi/Collection>

class OrgKdeAkonadiPOP3SettingsInterface;

namespace Akonadi
{
class Item;
}
class KJob;

class QString;

namespace MailCommon
{
/*!
 * The Util namespace contains a collection of helper functions use in
 * various places.
 */
namespace Util
{
[[nodiscard]] MAILCOMMON_EXPORT OrgKdeAkonadiPOP3SettingsInterface *createPop3SettingsInterface(const QString &ident);

[[nodiscard]] MAILCOMMON_EXPORT bool isVirtualCollection(const Akonadi::Collection &col);

[[nodiscard]] MAILCOMMON_EXPORT bool isVirtualCollection(const QString &resource);

[[nodiscard]] MAILCOMMON_EXPORT QString fullCollectionPath(const Akonadi::Collection &collection, bool addAccountName = true);
[[nodiscard]] MAILCOMMON_EXPORT QString fullCollectionRemoveIdPath(const Akonadi::Collection &collection, bool addAccountName = true);

MAILCOMMON_EXPORT bool showJobErrorMessage(KJob *job);

[[nodiscard]] MAILCOMMON_EXPORT Akonadi::AgentInstance::List agentInstances(bool excludeMailTransport = true);

[[nodiscard]] MAILCOMMON_EXPORT bool isMailAgent(const Akonadi::AgentInstance &instance, bool excludeMailTransport = true);

[[nodiscard]] MAILCOMMON_EXPORT bool isUnifiedMailboxesAgent(const Akonadi::Collection &col);

/*!
 * Returns the identity of the folder that contains the given Akonadi::Item.
 */
[[nodiscard]] MAILCOMMON_EXPORT uint folderIdentity(const Akonadi::Item &item);

[[nodiscard]] MAILCOMMON_EXPORT QString realFolderPath(const QString &path);

MAILCOMMON_EXPORT void expireOldMessages(const Akonadi::Collection &collection, bool immediate);

[[nodiscard]] MAILCOMMON_EXPORT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &folder);
[[nodiscard]] MAILCOMMON_EXPORT QString convertFolderPathToCollectionStr(const QString &folder);

MAILCOMMON_EXPORT void foundMailer(QStringList &lst, const QString &name);
[[nodiscard]] MAILCOMMON_EXPORT QStringList foundMailer();
[[nodiscard]] MAILCOMMON_EXPORT bool isLocalCollection(const QString &resource);
}
}
