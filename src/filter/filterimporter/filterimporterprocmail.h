/*
  Copyright (c) 2012-2020 Laurent Montel <montel@kde.org>

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

#ifndef MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERPROCMAIL_P_H
#define MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERPROCMAIL_P_H

#include "filter/filterimporter/filterimporterabstract.h"

#include <QTextStream>

class QFile;

namespace MailCommon {
class MailFilter;

class FilterImporterProcmail : public FilterImporterAbstract
{
public:
    explicit FilterImporterProcmail(QFile *file);
    FilterImporterProcmail(QString string);
    ~FilterImporterProcmail();
    static QString defaultFiltersSettingsPath();

private:
    Q_REQUIRED_RESULT MailCommon::MailFilter *parseLine(QTextStream &stream, QString line, MailCommon::MailFilter *filter);
    void readStream(QTextStream &stream);
    Q_REQUIRED_RESULT QString createUniqFilterName();
    int mFilterCount = 0;
};
}

#endif /*MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERPROCMAIL_P_H*/
