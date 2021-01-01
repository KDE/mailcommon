/*
  SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
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

void FilterImporterPathCache::insert(const QString &original, const Akonadi::Collection &newValue)
{
    if (original.isEmpty() || !newValue.isValid()) {
        return;
    }

    mFilterCache.insert(original, newValue);
}

Akonadi::Collection FilterImporterPathCache::convertedFilterPath(const QString &original)
{
    return mFilterCache.value(original);
}

void FilterImporterPathCache::clear()
{
    mFilterCache.clear();
}
