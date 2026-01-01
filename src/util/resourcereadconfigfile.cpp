/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "resourcereadconfigfile.h"

#include <KConfig>

using namespace MailCommon;

class MailCommon::ResourceReadConfigFilePrivate
{
public:
    ResourceReadConfigFilePrivate() = default;

    ~ResourceReadConfigFilePrivate()
    {
        delete mConfig;
    }

    KConfig *mConfig = nullptr;
};

ResourceReadConfigFile::ResourceReadConfigFile(const QString &resourceName)
    : d(new MailCommon::ResourceReadConfigFilePrivate)
{
    d->mConfig = new KConfig(resourceName + QStringLiteral("rc"));
}

ResourceReadConfigFile::~ResourceReadConfigFile() = default;

KConfigGroup ResourceReadConfigFile::group(const QString &name) const
{
    if (d->mConfig) {
        return d->mConfig->group(name);
    }
    return {};
}
