/*

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "foldersettings.h"
#include "kernel/mailkernel.h"
#include "mailcommon_debug.h"
#include "util/mailutil.h"
#include "util/resourcereadconfigfile.h"
#include <Akonadi/KMime/NewMailNotifierAttribute>
#include <CollectionModifyJob>
#include <ItemFetchJob>
#include <ItemFetchScope>
#include <PimCommon/PimUtil>

using namespace Akonadi;

#include <KIdentityManagement/Identity>
#include <KIdentityManagement/IdentityManager>

#include <QMutex>
#include <QMutexLocker>
#include <QSharedPointer>

namespace MailCommon
{
static QMutex mapMutex;
static QMap<Collection::Id, QSharedPointer<FolderSettings>> fcMap;

QSharedPointer<FolderSettings> FolderSettings::forCollection(const Akonadi::Collection &coll, bool writeConfig)
{
    QMutexLocker lock(&mapMutex);

    QSharedPointer<FolderSettings> sptr = fcMap.value(coll.id());

    if (!sptr) {
        sptr = QSharedPointer<FolderSettings>(new FolderSettings(coll, writeConfig));
        fcMap.insert(coll.id(), sptr);
    } else {
        sptr->setCollection(coll);
        if (!sptr->isWriteConfig() && writeConfig) {
            sptr->setWriteConfig(true);
        }
    }

    return sptr;
}

FolderSettings::FolderSettings(const Akonadi::Collection &col, bool writeconfig)
    : mCollection(col)
    , mWriteConfig(writeconfig)
{
    Q_ASSERT(col.isValid());
    mIdentity = KernelIf->identityManager()->defaultIdentity().uoid();

    readConfig();
    connect(KernelIf->identityManager(), QOverload<>::of(&KIdentityManagement::IdentityManager::changed), this, &FolderSettings::slotIdentitiesChanged);
}

FolderSettings::~FolderSettings()
{
    // qCDebug(MAILCOMMON_LOG)<<" FolderCollection::~FolderCollection"<<this;
    if (mWriteConfig) {
        writeConfig();
    }
}

MessageViewer::Viewer::DisplayFormatMessage FolderSettings::formatMessage() const
{
    return mFormatMessage;
}

void FolderSettings::setFormatMessage(MessageViewer::Viewer::DisplayFormatMessage formatMessage)
{
    mFormatMessage = formatMessage;
}

void FolderSettings::clearCache()
{
    QMutexLocker lock(&mapMutex);
    fcMap.clear();
}

void FolderSettings::resetHtmlFormat()
{
    QMutexLocker lock(&mapMutex);
    QMap<Collection::Id, QSharedPointer<FolderSettings>>::const_iterator i = fcMap.constBegin();
    while (i != fcMap.constEnd()) {
        i.value()->setFormatMessage(MessageViewer::Viewer::UseGlobalSetting);
        ++i;
    }
}

bool FolderSettings::isWriteConfig() const
{
    return mWriteConfig;
}

void FolderSettings::setWriteConfig(bool writeConfig)
{
    mWriteConfig = writeConfig;
}

QString FolderSettings::name() const
{
    return mCollection.name();
}

bool FolderSettings::isSystemFolder() const
{
    return Kernel::self()->isSystemFolderCollection(mCollection);
}

bool FolderSettings::isStructural() const
{
    return mCollection.contentMimeTypes().isEmpty();
}

bool FolderSettings::isReadOnly() const
{
    return mCollection.rights() & Akonadi::Collection::ReadOnly;
}

bool FolderSettings::canDeleteMessages() const
{
    return mCollection.rights() & Akonadi::Collection::CanDeleteItem;
}

bool FolderSettings::canCreateMessages() const
{
    return mCollection.rights() & Akonadi::Collection::CanCreateItem;
}

qint64 FolderSettings::count() const
{
    return mCollection.statistics().count();
}

Akonadi::Collection::Rights FolderSettings::rights() const
{
    return mCollection.rights();
}

Akonadi::CollectionStatistics FolderSettings::statistics() const
{
    return mCollection.statistics();
}

void FolderSettings::setCollection(const Akonadi::Collection &collection)
{
    mCollection = collection;
}

void FolderSettings::slotIdentitiesChanged()
{
    uint defaultIdentity = KernelIf->identityManager()->defaultIdentity().uoid();
    // The default identity may have changed, therefore set it again if necessary
    if (mUseDefaultIdentity) {
        mIdentity = defaultIdentity;
    }

    // Fall back to the default identity if the one used currently is invalid
    if (KernelIf->identityManager()->identityForUoid(mIdentity).isNull()) {
        mIdentity = defaultIdentity;
        mUseDefaultIdentity = true;
    }
}

QString FolderSettings::configGroupName(const Akonadi::Collection &col)
{
    return QStringLiteral("Folder-%1").arg(QString::number(col.id()));
}

void FolderSettings::readConfig()
{
    KConfigGroup configGroup(KernelIf->config(), configGroupName(mCollection));
    mMailingListEnabled = configGroup.readEntry("MailingListEnabled", false);
    mMailingList.readConfig(configGroup);

    mUseDefaultIdentity = configGroup.readEntry("UseDefaultIdentity", true);
    uint defaultIdentity = KernelIf->identityManager()->defaultIdentity().uoid();
    mIdentity = configGroup.readEntry("Identity", defaultIdentity);
    slotIdentitiesChanged();

    mPutRepliesInSameFolder = configGroup.readEntry("PutRepliesInSameFolder", false);
    mHideInSelectionDialog = configGroup.readEntry("HideInSelectionDialog", false);

    if (configGroup.hasKey(QStringLiteral("IgnoreNewMail"))) {
        if (configGroup.readEntry(QStringLiteral("IgnoreNewMail"), false)) {
            // migrate config.
            auto *newMailNotifierAttr = mCollection.attribute<Akonadi::NewMailNotifierAttribute>(Akonadi::Collection::AddIfMissing);
            newMailNotifierAttr->setIgnoreNewMail(true);
            new Akonadi::CollectionModifyJob(mCollection, this);
            // TODO verify if it works;
        }
        configGroup.deleteEntry("IgnoreNewMail");
    }

    const QString shortcut(configGroup.readEntry("Shortcut"));
    if (!shortcut.isEmpty()) {
        QKeySequence sc(shortcut);
        setShortcut(sc);
    }

    mFormatMessage = static_cast<MessageViewer::Viewer::DisplayFormatMessage>(
        configGroup.readEntry("displayFormatOverride", static_cast<int>(MessageViewer::Viewer::UseGlobalSetting)));

    mFolderHtmlLoadExtPreference = configGroup.readEntry("htmlLoadExternalOverride", false);
}

bool FolderSettings::isValid() const
{
    return mCollection.isValid();
}

void FolderSettings::writeConfig() const
{
    const QString res = resource();
    KConfigGroup configGroup(KernelIf->config(), configGroupName(mCollection));

    if (mMailingListEnabled) {
        configGroup.writeEntry("MailingListEnabled", mMailingListEnabled);
    } else {
        configGroup.deleteEntry("MailingListEnabled");
    }
    mMailingList.writeConfig(configGroup);

    if (!mUseDefaultIdentity) {
        configGroup.writeEntry("UseDefaultIdentity", mUseDefaultIdentity);
        uint defaultIdentityId = -1;

        if (PimCommon::Util::isImapResource(res)) {
            MailCommon::ResourceReadConfigFile resourceFile(res);
            KConfigGroup grp = resourceFile.group(QStringLiteral("cache"));
            if (grp.isValid()) {
                defaultIdentityId = grp.readEntry(QStringLiteral("AccountIdentity"), -1);
            }
        } else {
            defaultIdentityId = KernelIf->identityManager()->defaultIdentity().uoid();
        }

        if (mIdentity != defaultIdentityId) {
            configGroup.writeEntry("Identity", mIdentity);
        } else {
            configGroup.deleteEntry("Identity");
        }
    } else {
        configGroup.deleteEntry("Identity");
        configGroup.deleteEntry("UseDefaultIdentity");
    }

    if (mPutRepliesInSameFolder) {
        configGroup.writeEntry("PutRepliesInSameFolder", mPutRepliesInSameFolder);
    } else {
        configGroup.deleteEntry("PutRepliesInSameFolder");
    }
    if (mHideInSelectionDialog) {
        configGroup.writeEntry("HideInSelectionDialog", mHideInSelectionDialog);
    } else {
        configGroup.deleteEntry("HideInSelectionDialog");
    }

    if (!mShortcut.isEmpty()) {
        configGroup.writeEntry("Shortcut", mShortcut.toString());
    } else {
        configGroup.deleteEntry("Shortcut");
    }

    if (mFormatMessage != MessageViewer::Viewer::Unknown) {
        if (mFormatMessage == MessageViewer::Viewer::UseGlobalSetting) {
            configGroup.deleteEntry("displayFormatOverride");
        } else {
            configGroup.writeEntry("displayFormatOverride", static_cast<int>(mFormatMessage));
        }
    }
    if (mFolderHtmlLoadExtPreference) {
        configGroup.writeEntry("htmlLoadExternalOverride", mFolderHtmlLoadExtPreference);
    } else {
        configGroup.deleteEntry("htmlLoadExternalOverride");
    }
}

void FolderSettings::setShortcut(const QKeySequence &sc)
{
    mShortcut = sc;
}

const QKeySequence &FolderSettings::shortcut() const
{
    return mShortcut;
}

void FolderSettings::setUseDefaultIdentity(bool useDefaultIdentity)
{
    if (mUseDefaultIdentity != useDefaultIdentity) {
        mUseDefaultIdentity = useDefaultIdentity;
        if (mUseDefaultIdentity) {
            mIdentity = KernelIf->identityManager()->defaultIdentity().uoid();
        }
        KernelIf->syncConfig();
    }
}

bool FolderSettings::useDefaultIdentity() const
{
    return mUseDefaultIdentity;
}

void FolderSettings::setIdentity(uint identity)
{
    if (mIdentity != identity) {
        mIdentity = identity;
        KernelIf->syncConfig();
    }
}

QString FolderSettings::resource() const
{
    const QString resource = mCollection.resource();
    if (resource.isEmpty()) {
        const Collection col = CommonKernel->collectionFromId(mCollection.id());
        Q_ASSERT(col.isValid());
        Q_ASSERT(!col.resource().isEmpty());
        return col.resource();
    }
    return resource;
}

bool FolderSettings::folderHtmlLoadExtPreference() const
{
    return mFolderHtmlLoadExtPreference;
}

void FolderSettings::setFolderHtmlLoadExtPreference(bool folderHtmlLoadExtPreference)
{
    mFolderHtmlLoadExtPreference = folderHtmlLoadExtPreference;
}

uint FolderSettings::fallBackIdentity() const
{
    int identityId = -1;
    MailCommon::ResourceReadConfigFile resourceFile(resource());
    KConfigGroup grp = resourceFile.group(QStringLiteral("cache"));
    if (grp.isValid()) {
        const bool useDefault = grp.readEntry(QStringLiteral("UseDefaultIdentity"), true);
        if (useDefault) {
            return mIdentity;
        }
        const int remoteAccountIdent = grp.readEntry(QStringLiteral("AccountIdentity"), -1);
        if (remoteAccountIdent > 0) {
            identityId = remoteAccountIdent;
        }
    }
    if (identityId != -1 && !KernelIf->identityManager()->identityForUoid(identityId).isNull()) {
        return identityId;
    }
    return mIdentity;
}

uint FolderSettings::identity() const
{
    if (mUseDefaultIdentity) {
        return fallBackIdentity();
    }
    return mIdentity;
}

QString FolderSettings::mailingListPostAddress() const
{
    if (mMailingList.features() & MailingList::Post) {
        QList<QUrl> post = mMailingList.postUrls();
        QList<QUrl>::const_iterator end(post.constEnd());
        for (QList<QUrl>::const_iterator it = post.constBegin(); it != end; ++it) {
            // We check for isEmpty because before 3.3 postAddress was just an
            // email@kde.org and that leaves protocol() field in the qurl class
            const QString protocol = (*it).scheme();
            if (protocol == QLatin1String("mailto") || protocol.isEmpty()) {
                return (*it).path();
            }
        }
    }
    return QString();
}

void FolderSettings::setMailingListEnabled(bool enabled)
{
    if (mMailingListEnabled != enabled) {
        mMailingListEnabled = enabled;
        writeConfig();
    }
}

bool FolderSettings::isMailingListEnabled() const
{
    return mMailingListEnabled;
}

void FolderSettings::setMailingList(const MailingList &mlist)
{
    if (mMailingList == mlist) {
        return;
    }

    mMailingList = mlist;
    writeConfig();
}

MessageCore::MailingList FolderSettings::mailingList() const
{
    return mMailingList;
}

bool FolderSettings::putRepliesInSameFolder() const
{
    return mPutRepliesInSameFolder;
}

void FolderSettings::setPutRepliesInSameFolder(bool b)
{
    mPutRepliesInSameFolder = b;
}

bool FolderSettings::hideInSelectionDialog() const
{
    return mHideInSelectionDialog;
}

void FolderSettings::setHideInSelectionDialog(bool hide)
{
    mHideInSelectionDialog = hide;
}
}
