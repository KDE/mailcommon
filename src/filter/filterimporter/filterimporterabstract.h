/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <QList>
#include <QStringList>

class QFile;
class QDomDocument;

namespace MailCommon
{
class MailFilter;
/*!
 * \class MailCommon::FilterImporterAbstract
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FilterImporterAbstract
 *
 * \brief The FilterImporterAbstract class.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FilterImporterAbstract
{
public:
    /*!
     * Constructs a filter importer. If \a interactive is \c true, the user may be asked
     * to fix filter actions that refer to nonexistent entities.
     */
    explicit FilterImporterAbstract(bool interactive = true);
    /*!
     * Destroys the filter importer.
     */
    ~FilterImporterAbstract();
    /*!
     * Returns the list of imported filters.
     */
    [[nodiscard]] QList<MailFilter *> importFilter() const;
    /*!
     * Returns the names of the filters that were empty after import and were discarded.
     */
    [[nodiscard]] QStringList emptyFilter() const;

protected:
    /*!
     * Purifies \a filter and appends it to the list of imported filters,
     * or records its name as an empty filter.
     */
    void appendFilter(MailCommon::MailFilter *filter);
    /*!
     * Creates the filter action named \a actionName with the arguments \a value
     * and appends it to \a filter.
     */
    void createFilterAction(MailCommon::MailFilter *filter, const QString &actionName, const QString &value);
    /*!
     * Loads the content of \a file into \a doc.
     * Returns \c true on success.
     */
    bool loadDomElement(QDomDocument &doc, QFile *file);

    QList<MailFilter *> mListMailFilter;
    QStringList mEmptyFilter;

private:
    const bool mInteractive;
};
}
