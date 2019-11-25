/*
  Copyright (c) 2015-2019 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef FILTERIMPORTERGMAIL_H
#define FILTERIMPORTERGMAIL_H

#include "filter/filterimporter/filterimporterabstract.h"

class QFile;
class QDomElement;

namespace MailCommon {
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
    QString createUniqFilterName();
    void parseFilters(const QDomElement &e);
    int mFilterCount;
};
}

#endif // FILTERIMPORTERGMAIL_H
