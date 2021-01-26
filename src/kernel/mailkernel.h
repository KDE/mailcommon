/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_MAILKERNEL_H
#define MAILCOMMON_MAILKERNEL_H

#include "mailcommon/mailinterfaces.h"
#include "mailcommon_export.h"

#include <Akonadi/KMime/SpecialMailCollections>
#include <Collection>
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
    virtual ~Kernel();

    static Kernel *self();

    /**
     * Registers the interface dealing with main mail functionality. This function
     * MUST be called with a valid interface pointer, before any Kernel::self()
     * method is used. The pointer ownership will not be transferred to Kernel.
     */
    void registerKernelIf(IKernel *kernelIf);

    bool kernelIsRegistered() const;

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
    Akonadi::Collection collectionFromId(Akonadi::Collection::Id id) const;

    Akonadi::Collection inboxCollectionFolder();
    Akonadi::Collection outboxCollectionFolder();
    Akonadi::Collection sentCollectionFolder();
    Akonadi::Collection trashCollectionFolder();
    Akonadi::Collection draftsCollectionFolder();
    Akonadi::Collection templatesCollectionFolder();

    bool isSystemFolderCollection(const Akonadi::Collection &col);

    /**
     * Returns true if this folder is the inbox on the local disk
     */
    bool isMainFolderCollection(const Akonadi::Collection &col);

    /**
     * Returns true if the folder is either the outbox or one of the drafts-folders.
     */
    bool folderIsDraftOrOutbox(const Akonadi::Collection &collection);

    bool folderIsDrafts(const Akonadi::Collection &);

    bool folderIsTemplates(const Akonadi::Collection &collection);

    /**
     * Returns true if the folder is a trash folder.
     *
     * When calling this too early (before the SpecialMailCollectionsDiscoveryJob from initFolders finishes),
     * it will say false erroneously. However you can connect to SpecialMailCollections::collectionsChanged
     * to react on dynamic changes and call this again.
     */
    bool folderIsTrash(const Akonadi::Collection &collection);

    /**
     * Returns the trash folder for the resource which @p col belongs to.
     *
     * When calling this too early (before the SpecialMailCollectionsDiscoveryJob from initFolders finishes),
     * it will return an invalid collection erroneously. However you can connect to SpecialMailCollections::collectionsChanged
     * to react on dynamic changes and call this again.
     */
    Akonadi::Collection trashCollectionFromResource(const Akonadi::Collection &col);

    /**
     * Returns true if the folder is one of the sent-mail folders.
     */
    bool folderIsSentMailFolder(const Akonadi::Collection &);

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

    IKernel *mKernelIf;
    IFilter *mFilterIf;
    ISettings *mSettingsIf;
    PimCommon::ImapResourceCapabilitiesManager *mImapResourceManager;
};
}

#define KernelIf MailCommon::Kernel::self()->kernelIf()
#define FilterIf MailCommon::Kernel::self()->filterIf()
#define SettingsIf MailCommon::Kernel::self()->settingsIf()
#define CommonKernel MailCommon::Kernel::self()

#endif
