/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FILTERIMPORTERGMAIL_H
#define FILTERIMPORTERGMAIL_H

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
    Q_REQUIRED_RESULT QString createUniqFilterName();
    void parseFilters(const QDomElement &e);
    int mFilterCount = 0;
};
}

#endif // FILTERIMPORTERGMAIL_H
