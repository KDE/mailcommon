/*
  Copyright (C) 2017-2018 Montel Laurent <montel@kde.org>

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

#ifndef FILTERIMPORTERPATHCACHE_H
#define FILTERIMPORTERPATHCACHE_H

#include <QObject>
#include <QHash>
#include <AkonadiCore/Collection>
#include "mailcommon_export.h"
namespace MailCommon {
class MAILCOMMON_EXPORT FilterImporterPathCache : public QObject
{
    Q_OBJECT
public:
    static FilterImporterPathCache *self();

    explicit FilterImporterPathCache(QObject *parent = nullptr);
    ~FilterImporterPathCache();

    void insert(const QString &original, const Akonadi::Collection &newValue);
    Akonadi::Collection convertedFilterPath(const QString &original);
    void clear();
    int count() const;
private:
    QHash<QString, Akonadi::Collection> mFilterCache;
};
}

#endif // FILTERIMPORTERPATHCACHE_H
