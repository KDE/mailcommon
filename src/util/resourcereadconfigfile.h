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
     * Constructs a resource config file reader.
     *
     * \param resourceName The name of the resource to read configuration for
     */
    explicit ResourceReadConfigFile(const QString &resourceName);
    /*!
     * Destroys the resource config file reader.
     */
    ~ResourceReadConfigFile();

    /*!
     * Returns a configuration group with the given name.
     *
     * \param name The name of the configuration group
     * \return A KConfigGroup for accessing configuration data
     */
    KConfigGroup group(const QString &name) const;

private:
    std::unique_ptr<ResourceReadConfigFilePrivate> const d;
};
}
