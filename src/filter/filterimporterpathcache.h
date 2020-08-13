/*
  SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FILTERIMPORTERPATHCACHE_H
#define FILTERIMPORTERPATHCACHE_H

#include <QObject>
#include <QHash>
#include <AkonadiCore/Collection>
#include "mailcommon_export.h"
namespace MailCommon {
/**
 * @brief The FilterImporterPathCache class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FilterImporterPathCache : public QObject
{
    Q_OBJECT
public:
    static FilterImporterPathCache *self();

    explicit FilterImporterPathCache(QObject *parent = nullptr);
    ~FilterImporterPathCache();

    void insert(const QString &original, const Akonadi::Collection &newValue);
    Q_REQUIRED_RESULT Akonadi::Collection convertedFilterPath(const QString &original);
    void clear();
    Q_REQUIRED_RESULT int count() const;
private:
    QHash<QString, Akonadi::Collection> mFilterCache;
};
}

#endif // FILTERIMPORTERPATHCACHE_H
