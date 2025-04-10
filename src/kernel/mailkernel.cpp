/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mailkernel.h"
#include "mailcommon_debug.h"
#include "util/mailutil.h"
#include <PimCommon/PimUtil>
#include <PimCommonAkonadi/ImapResourceCapabilitiesManager>

#include "util/resourcereadconfigfile.h"
#include <Akonadi/AgentInstance>
#include <Akonadi/AgentManager>
#include <Akonadi/EntityMimeTypeFilterModel>
#include <Akonadi/SpecialMailCollectionsDiscoveryJob>
#include <Akonadi/SpecialMailCollectionsRequestJob>
#include <QApplication>

#include <KIdentityManagementCore/Identity>
#include <KIdentityManagementCore/IdentityManager>

#include <KLocalizedString>
#include <KMessageBox>

namespace MailCommon
{
class KernelPrivate
{
public:
    KernelPrivate()
        : kernel(new Kernel)
    {
    }

    ~KernelPrivate()
    {
        qCDebug(MAILCOMMON_LOG);
        delete kernel;
    }

    Kernel *const kernel;
};

Q_GLOBAL_STATIC(KernelPrivate, sInstance)

Kernel::Kernel(QObject *parent)
    : QObject(parent)
{
    mKernelIf = nullptr;
    mSettingsIf = nullptr;
    mFilterIf = nullptr;
    mImapResourceManager = new PimCommon::ImapResourceCapabilitiesManager(this);
}

Kernel::~Kernel()
{
    qCDebug(MAILCOMMON_LOG);
}

Kernel *Kernel::self()
{
    return sInstance->kernel; // will create it
}

void Kernel::registerKernelIf(IKernel *kernelIf)
{
    mKernelIf = kernelIf;
}

bool Kernel::kernelIsRegistered() const
{
    return mKernelIf != nullptr;
}

IKernel *Kernel::kernelIf() const
{
    Q_ASSERT(mKernelIf);
    return mKernelIf;
}

void Kernel::registerSettingsIf(ISettings *settingsIf)
{
    mSettingsIf = settingsIf;
}

ISettings *Kernel::settingsIf() const
{
    Q_ASSERT(mSettingsIf);
    return mSettingsIf;
}

void Kernel::registerFilterIf(IFilter *filterIf)
{
    mFilterIf = filterIf;
}

IFilter *Kernel::filterIf() const
{
    Q_ASSERT(mFilterIf);
    return mFilterIf;
}

PimCommon::ImapResourceCapabilitiesManager *Kernel::imapResourceManager() const
{
    return mImapResourceManager;
}

Akonadi::Collection Kernel::collectionFromId(Akonadi::Collection::Id id) const
{
    return Akonadi::EntityTreeModel::updatedCollection(kernelIf()->collectionModel(), id);
}

Akonadi::Collection Kernel::trashCollectionFolder()
{
    return Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Trash);
}

Akonadi::Collection Kernel::inboxCollectionFolder()
{
    return Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Inbox);
}

Akonadi::Collection Kernel::outboxCollectionFolder()
{
    return Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Outbox);
}

Akonadi::Collection Kernel::sentCollectionFolder()
{
    return Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::SentMail);
}

Akonadi::Collection Kernel::draftsCollectionFolder()
{
    return Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Drafts);
}

Akonadi::Collection Kernel::templatesCollectionFolder()
{
    return Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Templates);
}

Akonadi::Collection Kernel::spamsCollectionFolder()
{
    return Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Spam);
}

bool Kernel::isSystemFolderCollection(const Akonadi::Collection &col)
{
    return col == inboxCollectionFolder() || col == outboxCollectionFolder() || col == sentCollectionFolder() || col == trashCollectionFolder()
        || col == draftsCollectionFolder() || col == templatesCollectionFolder() || col == spamsCollectionFolder();
}

bool Kernel::isMainFolderCollection(const Akonadi::Collection &col)
{
    return col == inboxCollectionFolder();
}

//-----------------------------------------------------------------------------
void Kernel::initFolders()
{
    qCDebug(MAILCOMMON_LOG) << "Initialized and looking for specialcollection folders.";
    findCreateDefaultCollection(Akonadi::SpecialMailCollections::Inbox);
    findCreateDefaultCollection(Akonadi::SpecialMailCollections::Outbox);
    findCreateDefaultCollection(Akonadi::SpecialMailCollections::SentMail);
    findCreateDefaultCollection(Akonadi::SpecialMailCollections::Drafts);
    findCreateDefaultCollection(Akonadi::SpecialMailCollections::Trash);
    findCreateDefaultCollection(Akonadi::SpecialMailCollections::Templates);

    auto job = new Akonadi::SpecialMailCollectionsDiscoveryJob(this);
    job->start();
    // we don't connect to the job directly: it will register stuff into SpecialMailCollections, which will Q_EMIT signals.
}

void Kernel::findCreateDefaultCollection(Akonadi::SpecialMailCollections::Type type)
{
    if (Akonadi::SpecialMailCollections::self()->hasDefaultCollection(type)) {
        const Akonadi::Collection col = Akonadi::SpecialMailCollections::self()->defaultCollection(type);

        if (!(col.rights() & Akonadi::Collection::AllRights)) {
            emergencyExit(i18n("You do not have read/write permission to your inbox folder."));
        }
    } else {
        auto job = new Akonadi::SpecialMailCollectionsRequestJob(this);

        connect(job, &Akonadi::SpecialMailCollectionsRequestJob::result, this, &Kernel::createDefaultCollectionDone);

        job->requestDefaultCollection(type);
    }
}

void Kernel::createDefaultCollectionDone(KJob *job)
{
    if (job->error()) {
        emergencyExit(job->errorText());
        return;
    }

    auto requestJob = qobject_cast<Akonadi::SpecialMailCollectionsRequestJob *>(job);

    const Akonadi::Collection col = requestJob->collection();
    if (!(col.rights() & Akonadi::Collection::AllRights)) {
        emergencyExit(i18n("You do not have read/write permission to your inbox folder."));
    }
    Akonadi::SpecialMailCollections::self()->verifyI18nDefaultCollection(Akonadi::SpecialMailCollections::Inbox);
    Akonadi::SpecialMailCollections::self()->verifyI18nDefaultCollection(Akonadi::SpecialMailCollections::Outbox);
    Akonadi::SpecialMailCollections::self()->verifyI18nDefaultCollection(Akonadi::SpecialMailCollections::SentMail);
    Akonadi::SpecialMailCollections::self()->verifyI18nDefaultCollection(Akonadi::SpecialMailCollections::Drafts);
    Akonadi::SpecialMailCollections::self()->verifyI18nDefaultCollection(Akonadi::SpecialMailCollections::Trash);
    Akonadi::SpecialMailCollections::self()->verifyI18nDefaultCollection(Akonadi::SpecialMailCollections::Templates);

    connect(Akonadi::SpecialMailCollections::self(),
            &Akonadi::SpecialMailCollections::defaultCollectionsChanged,
            this,
            &Kernel::slotDefaultCollectionsChanged,
            Qt::UniqueConnection);
}

void Kernel::slotDefaultCollectionsChanged()
{
    disconnect(Akonadi::SpecialMailCollections::self(),
               &Akonadi::SpecialMailCollections::defaultCollectionsChanged,
               this,
               &Kernel::slotDefaultCollectionsChanged);
    initFolders();
}

void Kernel::emergencyExit(const QString &reason)
{
    QString mesg;
    if (reason.isEmpty()) {
        mesg = i18n("The Email program encountered a fatal error and will terminate now");
    } else {
        mesg = i18n(
            "The Email program encountered a fatal error and will terminate now.\n"
            "The error was:\n%1",
            reason);
    }

    qCWarning(MAILCOMMON_LOG) << mesg;

    // Show error box for the first error that caused emergencyExit.
    static bool s_showingErrorBox = false;
    if (!s_showingErrorBox) {
        s_showingErrorBox = true;
        if (qApp) { // see bug 313104
            KMessageBox::error(nullptr, mesg);
        }
        ::exit(1);
    }
}

bool Kernel::folderIsDraftOrOutbox(const Akonadi::Collection &col)
{
    if (col == Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Outbox)) {
        return true;
    }

    return folderIsDrafts(col);
}

bool Kernel::folderIsDrafts(const Akonadi::Collection &col)
{
    if (col == Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Drafts)) {
        return true;
    }

    const QString idString = QString::number(col.id());
    if (idString.isEmpty()) {
        return false;
    }

    // search the identities if the folder matches the drafts-folder
    const KIdentityManagementCore::IdentityManager *im = KernelIf->identityManager();
    KIdentityManagementCore::IdentityManager::ConstIterator end(im->end());
    for (KIdentityManagementCore::IdentityManager::ConstIterator it = im->begin(); it != end; ++it) {
        if ((*it).drafts() == idString) {
            return true;
        }
    }
    return false;
}

bool Kernel::folderIsTemplates(const Akonadi::Collection &col)
{
    if (col == Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Templates)) {
        return true;
    }
    const QString idString = QString::number(col.id());
    if (idString.isEmpty()) {
        return false;
    }

    // search the identities if the folder matches the templates-folder
    const KIdentityManagementCore::IdentityManager *im = KernelIf->identityManager();
    KIdentityManagementCore::IdentityManager::ConstIterator end(im->end());
    for (KIdentityManagementCore::IdentityManager::ConstIterator it = im->begin(); it != end; ++it) {
        if ((*it).templates() == idString) {
            return true;
        }
    }
    return false;
}

bool Kernel::folderIsSpams(const Akonadi::Collection &col)
{
    if (col == Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Spam)) {
        return true;
    }

    const QString idString = QString::number(col.id());
    if (idString.isEmpty()) {
        return false;
    }

    // search the identities if the folder matches the drafts-folder
    const KIdentityManagementCore::IdentityManager *im = KernelIf->identityManager();
    KIdentityManagementCore::IdentityManager::ConstIterator end(im->end());
    for (KIdentityManagementCore::IdentityManager::ConstIterator it = im->begin(); it != end; ++it) {
        if (!(*it).disabledSpam() && (*it).spam() == idString) {
            return true;
        }
    }
    return false;
}

Akonadi::Collection Kernel::trashCollectionFromResource(const Akonadi::Collection &col)
{
    Akonadi::Collection trashCol;
    if (col.isValid()) {
        const Akonadi::AgentInstance agent = Akonadi::AgentManager::self()->instance(col.resource());
        trashCol = Akonadi::SpecialMailCollections::self()->collection(Akonadi::SpecialMailCollections::Trash, agent);
    }
    return trashCol;
}

bool Kernel::folderIsTrash(const Akonadi::Collection &col)
{
    if (col == Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Trash)) {
        return true;
    }

    const Akonadi::AgentInstance::List lst = MailCommon::Util::agentInstances();
    for (const Akonadi::AgentInstance &agent : lst) {
        const Akonadi::Collection trash = Akonadi::SpecialMailCollections::self()->collection(Akonadi::SpecialMailCollections::Trash, agent);
        if (col == trash) {
            return true;
        }
    }
    return false;
}

bool Kernel::folderIsSentMailFolder(const Akonadi::Collection &col)
{
    if (col == Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::SentMail)) {
        return true;
    }
    const QString idString = QString::number(col.id());
    if (idString.isEmpty()) {
        return false;
    }

    // search the identities if the folder matches the sent-folder
    const KIdentityManagementCore::IdentityManager *im = KernelIf->identityManager();
    KIdentityManagementCore::IdentityManager::ConstIterator end(im->end());
    for (KIdentityManagementCore::IdentityManager::ConstIterator it = im->begin(); it != end; ++it) {
        if ((*it).fcc() == idString) {
            return true;
        }
    }
    return false;
}

bool Kernel::folderIsInbox(const Akonadi::Collection &collection)
{
    const QString collectionRemoteIdLower = collection.remoteId().toLower();
    if (collectionRemoteIdLower == QLatin1StringView("inbox") || collectionRemoteIdLower == QLatin1StringView("/inbox")
        || collectionRemoteIdLower == QLatin1StringView(".inbox") || collectionRemoteIdLower == QLatin1StringView("|inbox")) {
        return true;
    }
    // Fix order. Remoteid is not "inbox" when translated
    if (Akonadi::SpecialMailCollections::self()->specialCollectionType(collection) == Akonadi::SpecialMailCollections::Inbox) {
        return true;
    }

    // MBOX is one folder only, treat as inbox
    if (collection.resource().contains(MBOX_RESOURCE_IDENTIFIER)) {
        return true;
    }
    return false;
}

QMap<QString, Akonadi::Collection::Id> Kernel::pop3ResourceTargetCollection()
{
    QMap<QString, Akonadi::Collection::Id> mapIdentifierCollectionId;
    const Akonadi::AgentInstance::List lst = MailCommon::Util::agentInstances();
    for (const Akonadi::AgentInstance &type : lst) {
        if (type.status() == Akonadi::AgentInstance::Broken) {
            continue;
        }
        const QString typeIdentifier = type.identifier();
        if (typeIdentifier.contains(POP3_RESOURCE_IDENTIFIER)) {
            MailCommon::ResourceReadConfigFile resourceFile(typeIdentifier);
            const KConfigGroup grp = resourceFile.group(QStringLiteral("General"));
            if (grp.isValid()) {
                const Akonadi::Collection::Id targetCollection = grp.readEntry(QStringLiteral("targetCollection"), -1);
                mapIdentifierCollectionId.insert(typeIdentifier, targetCollection);
            }
        }
    }
    return mapIdentifierCollectionId;
}

#if MAILCOMMON_HAVE_ACTIVITY_SUPPORT
PimCommonActivities::ActivitiesBaseManager *Kernel::activitiesBaseManager() const
{
    return mActivitiesBaseManager;
}

void Kernel::registerActivitiesBaseManager(PimCommonActivities::ActivitiesBaseManager *manager)
{
    mActivitiesBaseManager = manager;
}
#endif
}

#include "moc_mailkernel.cpp"
