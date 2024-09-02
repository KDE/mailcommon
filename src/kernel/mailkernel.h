/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "config-mailcommon.h"
#include "mailcommon/mailinterfaces.h"
#include "mailcommon_export.h"

#include <Akonadi/Collection>
#include <Akonadi/SpecialMailCollections>
#include <KSharedConfig>

#include <QObject>
namespace PimCommon
{
class ImapResourceCapabilitiesManager;
}
namespace MailCommon
{
/**
 * Deals with common mail application related operations. The required interfaces
 * MUST be registered before using it!
 * Be careful when using in multi-threaded applications, as Kernel is a QObject
 * singleton, created in the main thread, thus event handling for Kernel::self()
 * will happen in the main thread.
 */

class MAILCOMMON_EXPORT Kernel : public QObject
{
    Q_OBJECT
public:
    ~Kernel() override;

    static Kernel *self();

    /**
     * Registers the interface dealing with main mail functionality. This function
     * MUST be called with a valid interface pointer, before any Kernel::self()
     * method is used. The pointer ownership will not be transferred to Kernel.
     */
    void registerKernelIf(IKernel *kernelIf);

    [[nodiscard]] bool kernelIsRegistered() const;

    IKernel *kernelIf() const;

    /**
     * Registers the interface dealing with mail settings. This function
     * MUST be called with a valid interface pointer, before any Kernel::self()
     * method is used. The pointer ownership will not be transferred to Kernel.
     */
    void registerSettingsIf(ISettings *settingsIf);

    ISettings *settingsIf() const;

    /**
     * Registers the interface dealing with mail settings. This function
     * MUST be called with a valid interface pointer, before any Kernel::self()
     * method is used. The pointer ownership will not be transferred to Kernel.
     */
    void registerFilterIf(IFilter *filterIf);

    IFilter *filterIf() const;

    /**
     * Returns the collection associated with the given @p id, or an invalid
     * collection if not found. The EntityTreeModel of the kernel is searched for
     * the collection. Since the ETM is loaded async, this method will not find
     * the collection right after startup, when the ETM is not yet fully loaded.
     */
    [[nodiscard]] Akonadi::Collection collectionFromId(Akonadi::Collection::Id id) const;

    [[nodiscard]] Akonadi::Collection inboxCollectionFolder();
    [[nodiscard]] Akonadi::Collection outboxCollectionFolder();
    [[nodiscard]] Akonadi::Collection sentCollectionFolder();
    [[nodiscard]] Akonadi::Collection trashCollectionFolder();
    [[nodiscard]] Akonadi::Collection draftsCollectionFolder();
    [[nodiscard]] Akonadi::Collection templatesCollectionFolder();

    [[nodiscard]] bool isSystemFolderCollection(const Akonadi::Collection &col);

    /**
     * Returns true if this folder is the inbox on the local disk
     */
    [[nodiscard]] bool isMainFolderCollection(const Akonadi::Collection &col);

    /**
     * Returns true if the folder is either the outbox or one of the drafts-folders.
     */
    [[nodiscard]] bool folderIsDraftOrOutbox(const Akonadi::Collection &collection);

    [[nodiscard]] bool folderIsDrafts(const Akonadi::Collection &);

    [[nodiscard]] bool folderIsTemplates(const Akonadi::Collection &collection);

    /**
     * Returns true if the folder is a trash folder.
     *
     * When calling this too early (before the SpecialMailCollectionsDiscoveryJob from initFolders finishes),
     * it will say false erroneously. However you can connect to SpecialMailCollections::collectionsChanged
     * to react on dynamic changes and call this again.
     */
    [[nodiscard]] bool folderIsTrash(const Akonadi::Collection &collection);

    /**
     * Returns the trash folder for the resource which @p col belongs to.
     *
     * When calling this too early (before the SpecialMailCollectionsDiscoveryJob from initFolders finishes),
     * it will return an invalid collection erroneously. However you can connect to SpecialMailCollections::collectionsChanged
     * to react on dynamic changes and call this again.
     */
    [[nodiscard]] Akonadi::Collection trashCollectionFromResource(const Akonadi::Collection &col);

    /**
     * Returns true if the folder is one of the sent-mail folders.
     */
    [[nodiscard]] bool folderIsSentMailFolder(const Akonadi::Collection &);

    static bool folderIsInbox(const Akonadi::Collection &);

    void initFolders();

    void emergencyExit(const QString &reason);

    PimCommon::ImapResourceCapabilitiesManager *imapResourceManager() const;

    static QMap<QString, Akonadi::Collection::Id> pop3ResourceTargetCollection();

private:
    void findCreateDefaultCollection(Akonadi::SpecialMailCollections::Type);

private Q_SLOTS:
    void createDefaultCollectionDone(KJob *job);
    void slotDefaultCollectionsChanged();

Q_SIGNALS:
    void requestConfigSync();
    void requestSystemTrayUpdate();

private:
    Kernel(QObject *parent = nullptr);
    friend class KernelPrivate;

    IKernel *mKernelIf = nullptr;
    IFilter *mFilterIf = nullptr;
    ISettings *mSettingsIf = nullptr;
    PimCommon::ImapResourceCapabilitiesManager *mImapResourceManager = nullptr;
};
}

#define KernelIf MailCommon::Kernel::self()->kernelIf()
#define FilterIf MailCommon::Kernel::self()->filterIf()
#define SettingsIf MailCommon::Kernel::self()->settingsIf()
#define CommonKernel MailCommon::Kernel::self()
