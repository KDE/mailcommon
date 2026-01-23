/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <KConfigGroup>
#include <QString>
#include <memory>

namespace MailCommon
{
class ResourceReadConfigFilePrivate;
/*!
 * \class MailCommon::ResourceReadConfigFile
 * \inmodule MailCommon
 * \inheaderfile MailCommon/ResourceReadConfigFile
 *
 * \brief The ResourceReadConfigFile class
 */
class MAILCOMMON_EXPORT ResourceReadConfigFile
{
public:
    /*!
     */
    explicit ResourceReadConfigFile(const QString &resourceName);
    /*!
     */
    ~ResourceReadConfigFile();

    /*!
     */
    KConfigGroup group(const QString &name) const;

private:
    std::unique_ptr<ResourceReadConfigFilePrivate> const d;
};
}
