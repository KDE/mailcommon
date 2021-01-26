/*
    SPDX-FileCopyrightText: 2011 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filtermanager.h"
#include "mailcommon_debug.h"

#include "filteractions/filteraction.h"
#include "filteractions/filteractiondict.h"
#include "filterimporterexporter.h"
#include "mailfilteragentinterface.h"
#include <KConfigGroup>
#include <Monitor>
#include <Tag>
#include <TagAttribute>
#include <TagFetchJob>
#include <TagFetchScope>

#include <QTimer>

namespace MailCommon
{
class Q_DECL_HIDDEN FilterManager::Private
{
public:
    Private(FilterManager *qq)
        : q(qq)
        , mMonitor(new Akonadi::Monitor)
    {
        const auto service = Akonadi::ServerManager::agentServiceName(Akonadi::ServerManager::Agent, QStringLiteral("akonadi_mailfilter_agent"));
        mMailFilterAgentInterface =
            new org::freedesktop::Akonadi::MailFilterAgent(service, QStringLiteral("/MailFilterAgent"), QDBusConnection::sessionBus(), q);
    }

    void readConfig();
    void writeConfig(bool withSync = true) const;
    void clear();

    QMap<QUrl, QString> mTagList;
    static FilterManager *mInstance;
    static FilterActionDict *mFilterActionDict;

    FilterManager *const q;
    OrgFreedesktopAkonadiMailFilterAgentInterface *mMailFilterAgentInterface = nullptr;
    QVector<MailCommon::MailFilter *> mFilters;
    Akonadi::Monitor *const mMonitor;
    bool mInitialized = false;
};

void FilterManager::Private::readConfig()
{
    KSharedConfig::Ptr config =
        KSharedConfig::openConfig(Akonadi::ServerManager::addNamespace(QStringLiteral("akonadi_mailfilter_agent")) + QStringLiteral("rc"));
    clear();
    QStringList emptyFilters;
    mFilters = FilterImporterExporter::readFiltersFromConfig(config, emptyFilters);
    Q_EMIT q->filtersChanged();
}

void FilterManager::Private::writeConfig(bool withSync) const
{
    KSharedConfig::Ptr config =
        KSharedConfig::openConfig(Akonadi::ServerManager::addNamespace(QStringLiteral("akonadi_mailfilter_agent")) + QStringLiteral("rc"));

    // Now, write out the new stuff:
    FilterImporterExporter::writeFiltersToConfig(mFilters, config);
    KConfigGroup group = config->group("General");

    if (withSync) {
        group.sync();
    }
}

void FilterManager::Private::clear()
{
    qDeleteAll(mFilters);
    mFilters.clear();
}
}

using namespace MailCommon;

FilterManager *FilterManager::Private::mInstance = nullptr;
FilterActionDict *FilterManager::Private::mFilterActionDict = nullptr;

FilterManager *FilterManager::instance()
{
    if (!FilterManager::Private::mInstance) {
        FilterManager::Private::mInstance = new FilterManager;
    }

    return FilterManager::Private::mInstance;
}

FilterActionDict *FilterManager::filterActionDict()
{
    if (!FilterManager::Private::mFilterActionDict) {
        FilterManager::Private::mFilterActionDict = new FilterActionDict;
    }

    return FilterManager::Private::mFilterActionDict;
}

FilterManager::FilterManager()
    : d(new Private(this))
{
    updateTagList();

    d->mMonitor->setObjectName(QStringLiteral("FilterManagerTagMonitor"));
    d->mMonitor->setTypeMonitored(Akonadi::Monitor::Tags);
    d->mMonitor->tagFetchScope().fetchAttribute<Akonadi::TagAttribute>();
    connect(d->mMonitor, &Akonadi::Monitor::tagAdded, this, &FilterManager::slotTagAdded);
    connect(d->mMonitor, &Akonadi::Monitor::tagRemoved, this, &FilterManager::slotTagRemoved);
    connect(d->mMonitor, &Akonadi::Monitor::tagChanged, this, &FilterManager::slotTagChanged);

    qDBusRegisterMetaType<QList<qint64>>();
    Akonadi::ServerManager::State state = Akonadi::ServerManager::self()->state();
    if (state == Akonadi::ServerManager::Running) {
        QTimer::singleShot(0, this, &FilterManager::slotReadConfig);
    } else {
        connect(Akonadi::ServerManager::self(), &Akonadi::ServerManager::stateChanged, this, &FilterManager::slotServerStateChanged);
    }
}

FilterManager::~FilterManager()
{
    cleanup();
}

void FilterManager::cleanup()
{
    d->clear();
}

void FilterManager::slotServerStateChanged(Akonadi::ServerManager::State state)
{
    if (state == Akonadi::ServerManager::Running) {
        d->readConfig();
        disconnect(Akonadi::ServerManager::self(), SIGNAL(stateChanged(Akonadi::ServerManager::State)));
    }
}

void FilterManager::updateTagList()
{
    auto fetchJob = new Akonadi::TagFetchJob(this);
    fetchJob->fetchScope().fetchAttribute<Akonadi::TagAttribute>();
    connect(fetchJob, &Akonadi::TagFetchJob::result, this, &FilterManager::slotFinishedTagListing);
}

bool FilterManager::initialized() const
{
    return d->mInitialized;
}

void FilterManager::slotReadConfig()
{
    d->readConfig();
    d->mInitialized = true;
    Q_EMIT loadingFiltersDone();
}

void FilterManager::slotFinishedTagListing(KJob *job)
{
    if (job->error()) {
        qCWarning(MAILCOMMON_LOG) << "failed to retrieve tags " << job->errorString();
    }
    auto fetchJob = static_cast<Akonadi::TagFetchJob *>(job);
    const Akonadi::Tag::List lstTags = fetchJob->tags();
    for (const Akonadi::Tag &tag : lstTags) {
        d->mTagList.insert(tag.url(), tag.name());
    }

    Q_EMIT tagListingFinished();
}

void FilterManager::slotTagAdded(const Akonadi::Tag &tag)
{
    d->mTagList.insert(tag.url(), tag.name());
    Q_EMIT tagListingFinished();
}

void FilterManager::slotTagChanged(const Akonadi::Tag &tag)
{
    if (d->mTagList.contains(tag.url())) {
        d->mTagList.insert(tag.url(), tag.name());
    }
    Q_EMIT tagListingFinished();
}

void FilterManager::slotTagRemoved(const Akonadi::Tag &tag)
{
    d->mTagList.remove(tag.url());
    Q_EMIT tagListingFinished();
}

QMap<QUrl, QString> FilterManager::tagList() const
{
    return d->mTagList;
}

bool FilterManager::isValid() const
{
    return d->mMailFilterAgentInterface->isValid();
}

QString FilterManager::createUniqueFilterName(const QString &name) const
{
    return d->mMailFilterAgentInterface->createUniqueName(name);
}

void FilterManager::showFilterLogDialog(qlonglong windowId)
{
    d->mMailFilterAgentInterface->showFilterLogDialog(windowId);
}

void FilterManager::filter(const Akonadi::Item &item, const QString &identifier, const QString &resourceId) const
{
    d->mMailFilterAgentInterface->filter(item.id(), identifier, resourceId);
}

void FilterManager::filter(const Akonadi::Item &item, FilterSet set, bool account, const QString &resourceId) const
{
    d->mMailFilterAgentInterface->filterItem(item.id(), static_cast<int>(set), account ? resourceId : QString());
}

void FilterManager::filter(const Akonadi::Collection &collection, FilterSet set) const
{
    filter(Akonadi::Collection::List{collection}, set);
}

void FilterManager::filter(const Akonadi::Collection::List &collections, FilterSet set) const
{
    QList<qint64> colIds;
    colIds.reserve(collections.size());
    for (const auto &col : collections) {
        colIds << col.id();
    }

    d->mMailFilterAgentInterface->filterCollections(colIds, static_cast<int>(set));
}

void FilterManager::filter(const Akonadi::Collection &collection, const QStringList &listFilters) const
{
    filter(Akonadi::Collection::List{collection}, listFilters);
}

void FilterManager::filter(const Akonadi::Collection::List &collections, const QStringList &listFilters, FilterSet set) const
{
    QList<qint64> colIds;
    colIds.reserve(collections.size());
    for (const auto &col : collections) {
        colIds << col.id();
    }

    d->mMailFilterAgentInterface->applySpecificFiltersOnCollections(colIds, listFilters, static_cast<int>(set));
}

void FilterManager::filter(const Akonadi::Item::List &messages, FilterManager::FilterSet set) const
{
    QList<qint64> itemIds;

    itemIds.reserve(messages.size());
    for (const Akonadi::Item &item : messages) {
        itemIds << item.id();
    }

    d->mMailFilterAgentInterface->filterItems(itemIds, static_cast<int>(set));
}

void FilterManager::filter(const Akonadi::Item::List &messages, SearchRule::RequiredPart requiredPart, const QStringList &listFilters) const
{
    QList<qint64> itemIds;

    itemIds.reserve(messages.size());
    for (const Akonadi::Item &item : messages) {
        itemIds << item.id();
    }
    d->mMailFilterAgentInterface->applySpecificFilters(itemIds, static_cast<int>(requiredPart), listFilters);
}

void FilterManager::setFilters(const QVector<MailCommon::MailFilter *> &filters)
{
    beginUpdate();
    d->clear();
    d->mFilters = filters;
    endUpdate();
}

QVector<MailCommon::MailFilter *> FilterManager::filters() const
{
    return d->mFilters;
}

void FilterManager::appendFilters(const QVector<MailCommon::MailFilter *> &filters, bool replaceIfNameExists)
{
    beginUpdate();
    if (replaceIfNameExists) {
        for (const MailCommon::MailFilter *newFilter : filters) {
            int numberOfFilters = d->mFilters.count();
            for (int i = 0; i < numberOfFilters; ++i) {
                MailCommon::MailFilter *filter = d->mFilters.at(i);
                if (newFilter->name() == filter->name()) {
                    d->mFilters.removeAll(filter);
                    i = 0;
                    numberOfFilters = d->mFilters.count();
                }
            }
        }
    }

    d->mFilters += filters;
    endUpdate();
}

void FilterManager::removeFilter(MailCommon::MailFilter *filter)
{
    beginUpdate();
    d->mFilters.removeAll(filter);
    endUpdate();
}

void FilterManager::beginUpdate()
{
}

void FilterManager::endUpdate()
{
    d->writeConfig(true);
    d->mMailFilterAgentInterface->reload();
    Q_EMIT filtersChanged();
}
