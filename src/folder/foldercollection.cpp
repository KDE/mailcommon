/*

  Copyright (c) 2009-2017 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "foldercollection.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"
#include "util/resourcereadconfigfile.h"
#include "PimCommon/PimUtil"
#include <AkonadiCore/NewMailNotifierAttribute>
#include "mailcommon_debug.h"
#include <ItemFetchJob>
#include <ItemFetchScope>
#include <CollectionModifyJob>

using namespace Akonadi;

#include <KIdentityManagement/IdentityManager>
#include <KIdentityManagement/Identity>

#include <QMutex>
#include <QMutexLocker>
#include <QSharedPointer>

namespace MailCommon
{

static QMutex mapMutex;
static QMap<Collection::Id, QSharedPointer<FolderCollection> > fcMap;

QSharedPointer<FolderCollection> FolderCollection::forCollection(
    const Akonadi::Collection &coll, bool writeConfig)
{
    QMutexLocker lock(&mapMutex);

    QSharedPointer<FolderCollection> sptr = fcMap.value(coll.id());

    if (!sptr) {
        sptr = QSharedPointer<FolderCollection>(new FolderCollection(coll, writeConfig));
        fcMap.insert(coll.id(), sptr);
    } else {
        sptr->setCollection(coll);
        if (!sptr->isWriteConfig() && writeConfig) {
            sptr->setWriteConfig(true);
        }
    }

    return sptr;
}

FolderCollection::FolderCollection(const Akonadi::Collection &col, bool writeconfig)
    : mCollection(col),
      mFormatMessage(MessageViewer::Viewer::Unknown),
      mPutRepliesInSameFolder(false),
      mHideInSelectionDialog(false),
      mWriteConfig(writeconfig)
{
    Q_ASSERT(col.isValid());
    mIdentity = KernelIf->identityManager()->defaultIdentity().uoid();

    readConfig();
    connect(KernelIf->identityManager(), SIGNAL(changed()),
            this, SLOT(slotIdentitiesChanged()));
}

FolderCollection::~FolderCollection()
{
    //qCDebug(MAILCOMMON_LOG)<<" FolderCollection::~FolderCollection"<<this;
    if (mWriteConfig) {
        writeConfig();
    }
}

MessageViewer::Viewer::DisplayFormatMessage FolderCollection::formatMessage() const
{
    return mFormatMessage;
}

void FolderCollection::setFormatMessage(MessageViewer::Viewer::DisplayFormatMessage formatMessage)
{
    mFormatMessage = formatMessage;
}

void FolderCollection::clearCache()
{
    QMutexLocker lock(&mapMutex);
    fcMap.clear();
}

void FolderCollection::resetHtmlFormat()
{
    QMutexLocker lock(&mapMutex);
    QMap<Collection::Id, QSharedPointer<FolderCollection> >::const_iterator i = fcMap.constBegin();
    while (i != fcMap.constEnd()) {
        i.value()->setFormatMessage(MessageViewer::Viewer::UseGlobalSetting);
        ++i;
    }
}

bool FolderCollection::isWriteConfig() const
{
    return mWriteConfig;
}

void FolderCollection::setWriteConfig(bool writeConfig)
{
    mWriteConfig = writeConfig;
}

QString FolderCollection::name() const
{
    return mCollection.name();
}

bool FolderCollection::isSystemFolder() const
{
    return Kernel::self()->isSystemFolderCollection(mCollection);
}

bool FolderCollection::isStructural() const
{
    return mCollection.contentMimeTypes().isEmpty();
}

bool FolderCollection::isReadOnly() const
{
    return mCollection.rights() & Akonadi::Collection::ReadOnly;
}

bool FolderCollection::canDeleteMessages() const
{
    return mCollection.rights() & Akonadi::Collection::CanDeleteItem;
}

bool FolderCollection::canCreateMessages() const
{
    return mCollection.rights() & Akonadi::Collection::CanCreateItem;
}

qint64 FolderCollection::count() const
{
    return mCollection.statistics().count();
}

Akonadi::Collection::Rights FolderCollection::rights() const
{
    return mCollection.rights();
}

Akonadi::CollectionStatistics FolderCollection::statistics() const
{
    return mCollection.statistics();
}

Akonadi::Collection FolderCollection::collection() const
{
    return mCollection;
}

void FolderCollection::setCollection(const Akonadi::Collection &collection)
{
    mCollection = collection;
}

void FolderCollection::slotIdentitiesChanged()
{
    uint defaultIdentity =  KernelIf->identityManager()->defaultIdentity().uoid();
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

QString FolderCollection::configGroupName(const Akonadi::Collection &col)
{
    return QStringLiteral("Folder-%1").arg(QString::number(col.id()));
}

void FolderCollection::readConfig()
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
            //migrate config.
            Akonadi::NewMailNotifierAttribute *newMailNotifierAttr = mCollection.attribute<Akonadi::NewMailNotifierAttribute>(Akonadi::Collection::AddIfMissing);
            newMailNotifierAttr->setIgnoreNewMail(true);
            new Akonadi::CollectionModifyJob(mCollection, this);
            //TODO verify if it works;
        }
        configGroup.deleteEntry("IgnoreNewMail");
    }

    const QString shortcut(configGroup.readEntry("Shortcut"));
    if (!shortcut.isEmpty()) {
        QKeySequence sc(shortcut);
        setShortcut(sc);
    }

    mFormatMessage = static_cast<MessageViewer::Viewer::DisplayFormatMessage>(configGroup.readEntry("displayFormatOverride",
                     static_cast<int>(MessageViewer::Viewer::UseGlobalSetting)));
}

bool FolderCollection::isValid() const
{
    return mCollection.isValid();
}

void FolderCollection::writeConfig() const
{
    KConfigGroup configGroup(KernelIf->config(), configGroupName(mCollection));

    configGroup.writeEntry("MailingListEnabled", mMailingListEnabled);
    mMailingList.writeConfig(configGroup);

    configGroup.writeEntry("UseDefaultIdentity", mUseDefaultIdentity);

    if (!mUseDefaultIdentity) {
        uint defaultIdentityId = -1;

        if (PimCommon::Util::isImapResource(mCollection.resource())) {
            PimCommon::ResourceReadConfigFile resourceFile(mCollection.resource());
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
    }

    configGroup.writeEntry("PutRepliesInSameFolder", mPutRepliesInSameFolder);
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
}

void FolderCollection::setShortcut(const QKeySequence &sc)
{
    if (mShortcut != sc) {
        mShortcut = sc;
    }
}

const QKeySequence &FolderCollection::shortcut() const
{
    return mShortcut;
}

void FolderCollection::setUseDefaultIdentity(bool useDefaultIdentity)
{
    if (mUseDefaultIdentity != useDefaultIdentity) {
        mUseDefaultIdentity = useDefaultIdentity;
        if (mUseDefaultIdentity) {
            mIdentity = KernelIf->identityManager()->defaultIdentity().uoid();
        }
        KernelIf->syncConfig();
    }
}

bool FolderCollection::useDefaultIdentity() const
{
    return mUseDefaultIdentity;
}

void FolderCollection::setIdentity(uint identity)
{
    if (mIdentity != identity) {
        mIdentity = identity;
        KernelIf->syncConfig();
    }
}

uint FolderCollection::fallBackIdentity() const
{
    int identityId = -1;
    PimCommon::ResourceReadConfigFile resourceFile(mCollection.resource());
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
    if (identityId != -1 &&
            !KernelIf->identityManager()->identityForUoid(identityId).isNull()) {
        return identityId;
    }
    return identityId;
}

uint FolderCollection::identity() const
{
    if (mUseDefaultIdentity) {
        return fallBackIdentity();
    }
    return mIdentity;
}

QString FolderCollection::mailingListPostAddress() const
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

void FolderCollection::setMailingListEnabled(bool enabled)
{
    if (mMailingListEnabled != enabled) {
        mMailingListEnabled = enabled;
        writeConfig();
    }
}

bool FolderCollection::isMailingListEnabled() const
{
    return mMailingListEnabled;
}

void FolderCollection::setMailingList(const MailingList &mlist)
{
    if (mMailingList == mlist) {
        return;
    }

    mMailingList = mlist;
    writeConfig();
}

MessageCore::MailingList FolderCollection::mailingList() const
{
    return mMailingList;
}

bool FolderCollection::putRepliesInSameFolder() const
{
    return mPutRepliesInSameFolder;
}

void FolderCollection::setPutRepliesInSameFolder(bool b)
{
    mPutRepliesInSameFolder = b;
}

bool FolderCollection::hideInSelectionDialog() const
{
    return mHideInSelectionDialog;
}

void FolderCollection::setHideInSelectionDialog(bool hide)
{
    mHideInSelectionDialog = hide;
}

}

