/*
  Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "filterimporterpathcache.h"

using namespace MailCommon;
FilterImporterPathCache::FilterImporterPathCache(QObject *parent)
    : QObject(parent)
{

}

FilterImporterPathCache::~FilterImporterPathCache()
{

}

FilterImporterPathCache *FilterImporterPathCache::self()
{
    static FilterImporterPathCache s_self;
    return &s_self;
}

int FilterImporterPathCache::count() const
{
    return mFilterCache.count();
}

void FilterImporterPathCache::insert(const QString &original, const QString &newValue)
{
    if (original.isEmpty() || newValue.isEmpty()) {
        return;
    }

    mFilterCache.insert(original, newValue);
}

QString FilterImporterPathCache::convertedFilterPath(const QString &original)
{
    return mFilterCache.value(original);
}

void FilterImporterPathCache::clear()
{
    mFilterCache.clear();
}
