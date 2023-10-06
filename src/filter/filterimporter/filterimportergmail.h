/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "filter/filterimporter/filterimporterabstract.h"

class QFile;
class QDomElement;

namespace MailCommon
{
/**
 * @brief The FilterImporterGmail class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FilterImporterGmail : public FilterImporterAbstract
{
public:
    explicit FilterImporterGmail(QFile *file);
    ~FilterImporterGmail();
    static QString defaultFiltersSettingsPath();

private:
    [[nodiscard]] MAILCOMMON_NO_EXPORT QString createUniqFilterName();
    void parseFilters(const QDomElement &e);
    int mFilterCount = 0;
};
}
