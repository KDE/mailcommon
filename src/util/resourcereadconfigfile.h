/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef RESOURCEREADCONFIGFILE_H
#define RESOURCEREADCONFIGFILE_H

#include "mailcommon_export.h"
#include <KConfigGroup>
#include <QString>

namespace MailCommon
{
class ResourceReadConfigFilePrivate;
class MAILCOMMON_EXPORT ResourceReadConfigFile
{
public:
    ResourceReadConfigFile(const QString &resourceName);
    ~ResourceReadConfigFile();

    KConfigGroup group(const QString &name) const;

private:
    ResourceReadConfigFilePrivate *const d;
};
}

#endif // RESOURCEREADCONFIGFILE_H
