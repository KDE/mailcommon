/*
  SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/Collection>
#include <QHash>
#include <QObject>
namespace MailCommon
{
/*!
 * \class MailCommon::FilterImporterPathCache
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FilterImporterPathCache
 *
 * \brief The FilterImporterPathCache class.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FilterImporterPathCache : public QObject
{
    Q_OBJECT
public:
    /*!
     * Returns the global instance of the path cache.
     */
    static FilterImporterPathCache *self();

    /*!
     * Constructs a path cache with the given \a parent.
     */
    explicit FilterImporterPathCache(QObject *parent = nullptr);
    /*!
     * Destroys the path cache.
     */
    ~FilterImporterPathCache() override;

    /*!
     * Associates the imported folder path \a original with the collection \a newValue.
     */
    void insert(const QString &original, const Akonadi::Collection &newValue);
    /*!
     * Returns the collection associated with the imported folder path \a original.
     */
    [[nodiscard]] Akonadi::Collection convertedFilterPath(const QString &original);
    /*!
     * Clears the cache.
     */
    void clear();
    /*!
     * Returns the number of cached paths.
     */
    [[nodiscard]] int count() const;

private:
    QHash<QString, Akonadi::Collection> mFilterCache;
};
}
