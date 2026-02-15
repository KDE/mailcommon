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

#if MAILCOMMON_HAVE_ACTIVITY_SUPPORT
namespace PimCommonActivities
{
class ActivitiesBaseManager;
}
#endif

namespace MailCommon
{
/*!
 * \class MailCommon::Kernel
 * \inmodule MailCommon
 * \inheaderfile MailCommon/Kernel
 *
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
    /*!
     * Destroys the kernel.
     */
    ~Kernel() override;

    /*!
     * Returns the kernel singleton.
     *
     * \return A pointer to the Kernel instance
     */
    static Kernel *self();

    /*!
     * Registers the interface dealing with main mail functionality. This function
     * MUST be called with a valid interface pointer, before any Kernel::self()
     * method is used. The pointer ownership will not be transferred to Kernel.
     *
     * \param kernelIf The kernel interface to register
     */
    void registerKernelIf(IKernel *kernelIf);

    /*!
     * Returns whether a kernel interface is registered.
     *
     * \return True if registered, false otherwise
     */
    [[nodiscard]] bool kernelIsRegistered() const;

    /*!
     * Returns the registered kernel interface.
     *
     * \return A pointer to the kernel interface
     */
    IKernel *kernelIf() const;

    /*!
     * Registers the interface dealing with mail settings. This function
     * MUST be called with a valid interface pointer, before any Kernel::self()
     * method is used. The pointer ownership will not be transferred to Kernel.
     *
     * \param settingsIf The settings interface to register
     */
    void registerSettingsIf(ISettings *settingsIf);

    /*!
     * Returns the registered settings interface.
     *
     * \return A pointer to the settings interface
     */
    ISettings *settingsIf() const;

    /*!
     * Registers the interface dealing with mail settings. This function
     * MUST be called with a valid interface pointer, before any Kernel::self()
     * method is used. The pointer ownership will not be transferred to Kernel.
     *
     * \param filterIf The filter interface to register
     */
    void registerFilterIf(IFilter *filterIf);

    /*!
     * Returns the registered filter interface.
     *
     * \return A pointer to the filter interface
     */
    IFilter *filterIf() const;

    /*!
     * Returns the collection associated with the given \a id, or an invalid
     * collection if not found. The EntityTreeModel of the kernel is searched for
     * the collection. Since the ETM is loaded async, this method will not find
     * the collection right after startup, when the ETM is not yet fully loaded.
     *
     * \param id The collection ID to look up
     * \return The collection with the given ID, or an invalid collection
     */
    [[nodiscard]] Akonadi::Collection collectionFromId(Akonadi::Collection::Id id) const;

    /*!
     * Returns the inbox collection folder.
     *
     * \return The inbox collection
     */
    [[nodiscard]] Akonadi::Collection inboxCollectionFolder();
    /*!
     * Returns the outbox collection folder.
     *
     * \return The outbox collection
     */
    [[nodiscard]] Akonadi::Collection outboxCollectionFolder();
    /*!
     * Returns the sent mail collection folder.
     *
     * \return The sent mail collection
     */
    [[nodiscard]] Akonadi::Collection sentCollectionFolder();
    /*!
     * Returns the trash collection folder.
     *
     * \return The trash collection
     */
    [[nodiscard]] Akonadi::Collection trashCollectionFolder();
    /*!
     * Returns the drafts collection folder.
     *
     * \return The drafts collection
     */
    [[nodiscard]] Akonadi::Collection draftsCollectionFolder();
    /*!
     * Returns the templates collection folder.
     *
     * \return The templates collection
     */
    [[nodiscard]] Akonadi::Collection templatesCollectionFolder();

    /*!
     * Returns the spam collection folder.
     *
     * \return The spam collection
     */
    [[nodiscard]] Akonadi::Collection spamsCollectionFolder();

    /*!
     * Checks if the given collection is a system folder collection.
     *
     * \param col The collection to check
     * \return True if the collection is a system folder, false otherwise
     */
    [[nodiscard]] bool isSystemFolderCollection(const Akonadi::Collection &col);

    /*!
     * Returns true if this folder is the inbox on the local disk
     *
     * \param col The collection to check
     * \return True if the collection is the main inbox, false otherwise
     */
    [[nodiscard]] bool isMainFolderCollection(const Akonadi::Collection &col);

    /*!
     * Returns true if the folder is either the outbox or one of the drafts-folders.
     *
     * \param collection The collection to check
     * \return True if the collection is a draft or outbox folder, false otherwise
     */
    [[nodiscard]] bool folderIsDraftOrOutbox(const Akonadi::Collection &collection);

    /*!
     * Checks if the given collection is a drafts folder.
     *
     * \param collection The collection to check
     * \return True if the collection is a drafts folder, false otherwise
     */
    [[nodiscard]] bool folderIsDrafts(const Akonadi::Collection &collection);

    /*!
     * Checks if the given collection is a templates folder.
     *
     * \param collection The collection to check
     * \return True if the collection is a templates folder, false otherwise
     */
    [[nodiscard]] bool folderIsTemplates(const Akonadi::Collection &collection);

    /*!
     * Checks if the given collection is a spam/junk folder.
     *
     * \param collection The collection to check
     * \return True if the collection is a spam folder, false otherwise
     */
    [[nodiscard]] bool folderIsSpams(const Akonadi::Collection &collection);

    /*!
     * Returns true if the folder is a trash folder.
     *
     * When calling this too early (before the SpecialMailCollectionsDiscoveryJob from initFolders finishes),
     * it will say false erroneously. However you can connect to SpecialMailCollections::collectionsChanged
     * to react on dynamic changes and call this again.
     *
     * \param collection The collection to check
     * \return True if the collection is a trash folder, false otherwise
     */
    [[nodiscard]] bool folderIsTrash(const Akonadi::Collection &collection);

    /*!
     * Returns the trash folder for the resource which \a col belongs to.
     *
     * When calling this too early (before the SpecialMailCollectionsDiscoveryJob from initFolders finishes),
     * it will return an invalid collection erroneously. However you can connect to SpecialMailCollections::collectionsChanged
     * to react on dynamic changes and call this again.
     *
     * \param col The collection whose resource's trash folder should be returned
     * \return The trash collection for the resource
     */
    [[nodiscard]] Akonadi::Collection trashCollectionFromResource(const Akonadi::Collection &col);

    /*!
     * Returns true if the folder is one of the sent-mail folders.
     *
     * \param collection The collection to check
     * \return True if the collection is a sent mail folder, false otherwise
     */
    [[nodiscard]] bool folderIsSentMailFolder(const Akonadi::Collection &collection);

    /*!
     * Checks if the given collection is an inbox folder.
     *
     * \param collection The collection to check
     * \return True if the collection is an inbox, false otherwise
     */
    static bool folderIsInbox(const Akonadi::Collection &collection);

    /*!
     * Initializes the kernel and system mail folders.
     */
    void initFolders();

    /*!
     * Performs emergency exit when an error occurs.
     *
     * \param reason The reason for the emergency exit
     */
    void emergencyExit(const QString &reason);

    /*!
     * Returns the IMAP resource capabilities manager.
     *
     * \return A pointer to the IMAP resource capabilities manager
     */
    PimCommon::ImapResourceCapabilitiesManager *imapResourceManager() const;

    /*!
     * Returns the mapping of POP3 resources to their target collections.
     *
     * \return A map from resource names to collection IDs
     */
    static QMap<QString, Akonadi::Collection::Id> pop3ResourceTargetCollection();

#if MAILCOMMON_HAVE_ACTIVITY_SUPPORT
    /*!
     * Returns the activities base manager.
     *
     * \return A pointer to the activities base manager
     */
    PimCommonActivities::ActivitiesBaseManager *activitiesBaseManager() const;
    /*!
     * Registers the activities base manager.
     *
     * \param manager The activities base manager to register
     */
    void registerActivitiesBaseManager(PimCommonActivities::ActivitiesBaseManager *manager);
#endif

private:
    void findCreateDefaultCollection(Akonadi::SpecialMailCollections::Type);

private Q_SLOTS:
    void createDefaultCollectionDone(KJob *job);
    void slotDefaultCollectionsChanged();

Q_SIGNALS:
    /*!
     * Emitted when a configuration synchronization is requested.
     */
    void requestConfigSync();
    /*!
     * Emitted when a system tray update is requested.
     */
    void requestSystemTrayUpdate();

private:
    Kernel(QObject *parent = nullptr);
    friend class KernelPrivate;

    IKernel *mKernelIf = nullptr;
    IFilter *mFilterIf = nullptr;
    ISettings *mSettingsIf = nullptr;
    PimCommon::ImapResourceCapabilitiesManager *mImapResourceManager = nullptr;
#if MAILCOMMON_HAVE_ACTIVITY_SUPPORT
    PimCommonActivities::ActivitiesBaseManager *mActivitiesBaseManager = nullptr;
#endif
};
}

#define KernelIf MailCommon::Kernel::self()->kernelIf()
#define FilterIf MailCommon::Kernel::self()->filterIf()
#define SettingsIf MailCommon::Kernel::self()->settingsIf()
#define CommonKernel MailCommon::Kernel::self()
