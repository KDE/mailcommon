/*
  SPDX-FileCopyrightText: 2003 Andreas Gungl <a.gungl@gmx.de>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "mailcommon_export.h"

#include <QObject>
#include <QStringList>
#include <memory>
namespace MailCommon
{
/**
 * @short KMail Filter Log Collector.
 *
 * The filter log helps to collect log information about the
 * filter process in KMail. It's implemented as singleton,
 * so it's easy to direct pieces of information to a unique
 * instance.
 * It's possible to activate / deactivate logging. All
 * collected log information can get thrown away, the
 * next added log entry is the first one until another
 * clearing.
 * A signal is emitted whenever a new logentry is added,
 * when the log was cleared or any log state was changed.
 *
 * @author Andreas Gungl <a.gungl@gmx.de>
 */
class MAILCOMMON_EXPORT FilterLog : public QObject
{
    Q_OBJECT

public:
    /**
     * Destroys the filter log.
     */
    ~FilterLog() override;

    /**
     * Returns the single global instance of the filter log.
     */
    static FilterLog *instance();

    /**
     * Describes the type of content that will be logged.
     */
    enum ContentType {
        Meta = 1, ///< Log all meta data.
        PatternDescription = 2, ///< Log all pattern description.
        RuleResult = 4, ///< Log all rule matching results.
        PatternResult = 8, ///< Log all pattern matching results.
        AppliedAction = 16 ///< Log all applied actions.
    };

    /**
     * Sets whether the filter log is currently @p active.
     */
    void setLogging(bool active);

    /**
     * Returns whether the filter log is currently active.
     */
    [[nodiscard]] bool isLogging() const;

    /**
     * Sets the maximum @p size of the log in bytes.
     */
    void setMaxLogSize(long size = -1);

    /**
     * Returns the maximum size of the log in bytes.
     */
    [[nodiscard]] long maxLogSize() const;

    /**
     * Sets whether a given content @p type will be @p enabled for logging.
     */
    void setContentTypeEnabled(ContentType type, bool enabled);

    /**
     * Returns whether the given content @p type is enabled for logging.
     */
    [[nodiscard]] bool isContentTypeEnabled(ContentType type) const;

    /**
     * Adds the given log @p entry under the given content @p type to the log.
     */
    void add(const QString &entry, ContentType type);

    /**
     * Adds a separator line to the log.
     */
    void addSeparator();

    /**
     * Clears the log.
     */
    void clear();

    /**
     * Returns the list of log entries.
     */
    [[nodiscard]] QStringList logEntries() const;

    /**
     * Saves the log to the file with the given @p fileName.
     *
     * @return @c true on success or @c false on failure.
     */
    bool saveToFile(const QString &fileName) const;

    /**
     * Returns an escaped version of the log which can be used
     * in a HTML document.
     */
    [[nodiscard]] static QString recode(const QString &plain);

    /**
     * Dumps the log to console. Used for debugging.
     */
    void dump();

Q_SIGNALS:
    /**
     * This signal is emitted whenever a new @p entry has been added to the log.
     */
    void logEntryAdded(const QString &entry);

    /**
     * This signal is emitted whenever the log has shrunk.
     */
    void logShrinked();

    /**
     * This signal is emitted whenever the activity of the filter log has been changed.
     */
    void logStateChanged();

private:
    //@cond PRIVATE
    MAILCOMMON_NO_EXPORT FilterLog();

    class FilterLogPrivate;
    std::unique_ptr<FilterLogPrivate> const d;
    //@endcond
};
}
