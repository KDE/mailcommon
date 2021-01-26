/*
  SPDX-FileCopyrightText: 2011 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MAILCOMMON_FILTERMANAGER_H
#define MAILCOMMON_FILTERMANAGER_H

#include "mailcommon_export.h"
#include "mailfilter.h"

#include <Item>
#include <ServerManager>

#include <QObject>

namespace MailCommon
{
class FilterActionDict;

/**
 * @short A wrapper class that allows easy access to the mail filters
 *
 * This class communicates with the mailfilter agent via DBus.
 */
class MAILCOMMON_EXPORT FilterManager : public QObject
{
    Q_OBJECT

public:
    ~FilterManager();
    /**
     * Describes the list of filters.
     */
    enum FilterSet {
        NoSet = 0x0,
        Inbound = 0x1,
        Outbound = 0x2,
        Explicit = 0x4,
        BeforeOutbound = 0x8,
        AllFolders = 0x16, ///< Apply the filter on all folders, not just inbox
        All = Inbound | BeforeOutbound | Outbound | Explicit | AllFolders
    };

    /**
     * Returns the global filter manager object.
     */
    static FilterManager *instance();

    /**
     * Returns whether the filter manager is in a usable state.
     */
    bool isValid() const;

    /**
     * Checks for existing filters with the @p name and extend the
     * "name" to "name (i)" until no match is found for i=1..n
     */
    QString createUniqueFilterName(const QString &name) const;

    /**
     * Returns the global filter action dictionary.
     */
    static FilterActionDict *filterActionDict();

    /**
     * Shows the filter log dialog.
     *
     * This is used to debug problems with filters.
     */
    void showFilterLogDialog(qlonglong windowId);

    /// Apply filters interface

    /**
     * Applies filter with the given @p identifier on the message @p item.
     * @return @c true on success, @c false otherwise.
     */
    void filter(const Akonadi::Item &item, const QString &identifier, const QString &resourceId) const;

    /**
     * Process given message item by applying the filter rules one by
     * one. You can select which set of filters (incoming or outgoing)
     * should be used.
     *
     * @param item The message item to process.
     * @param set Select the filter set to use.
     * @param account @c true if an account id is specified else @c false
     * @param accountId The id of the resource that the message was retrieved from
     */
    void filter(const Akonadi::Item &item, FilterSet set = Inbound, bool account = false, const QString &resourceId = QString()) const;

    /**
     * Process all messages in given collection by applying the filters rules one
     * by one. You can select which set of filters (incoming or outgoing)
     * should be used.
     */
    void filter(const Akonadi::Collection &collection, FilterSet set = Explicit) const;

    /**
     * Apply specified filters on all messages in given collection
     */
    void filter(const Akonadi::Collection &collection, const QStringList &listFilters) const;

    /**
     * Process all messages in given collections by applying the filters rules one
     * by one. You can select which set of filters (incoming or outgoing)
     * should be used.
     */
    void filter(const Akonadi::Collection::List &collections, FilterSet set = Explicit) const;

    /**
     * Apply specified filters on all messages in given collection
     */
    void filter(const Akonadi::Collection::List &collections, const QStringList &listFilters, FilterSet set = Explicit) const;

    /**
     * Process given @p messages by applying the filter rules one by
     * one. You can select which set of filters (incoming or outgoing)
     * should be used.
     *
     * @param item The message item to process.
     * @param set Select the filter set to use.
     */
    void filter(const Akonadi::Item::List &messages, FilterSet set = Explicit) const;

    void filter(const Akonadi::Item::List &messages, SearchRule::RequiredPart requiredPart, const QStringList &listFilters) const;

    /// Manage filters interface

    /**
     * Appends the list of @p filters to the current list of filters and
     * write everything back into the configuration. The filter manager
     * takes ownership of the filters in the list.
     */
    void appendFilters(const QVector<MailCommon::MailFilter *> &filters, bool replaceIfNameExists = false);

    /**
     * Removes the given @p filter from the list.
     * The filter object is not deleted.
     */
    void removeFilter(MailCommon::MailFilter *filter);

    /**
     * Replace the list of filters of the filter manager with the given list of @p filters.
     * The manager takes ownership of the filters.
     */
    void setFilters(const QVector<MailCommon::MailFilter *> &filters);

    /**
     * Returns the filter list of the manager.
     */
    QVector<MailCommon::MailFilter *> filters() const;

    /**
     * Should be called at the beginning of an filter list update.
     */
    void beginUpdate();

    /**
     * Should be called at the end of an filter list update.
     */
    void endUpdate();

    QMap<QUrl, QString> tagList() const;

    bool initialized() const;

    void cleanup();
private Q_SLOTS:
    void slotServerStateChanged(Akonadi::ServerManager::State);
    void slotFinishedTagListing(KJob *);
    void slotReadConfig();
    void updateTagList();

    void slotTagAdded(const Akonadi::Tag &);
    void slotTagChanged(const Akonadi::Tag &);
    void slotTagRemoved(const Akonadi::Tag &);

Q_SIGNALS:
    /**
     * This signal is emitted whenever the filter list has been updated.
     */
    void filtersChanged();

    void tagListingFinished();

    void loadingFiltersDone();

private:
    FilterManager();

    class Private;
    Private *const d;
};
}

#endif
