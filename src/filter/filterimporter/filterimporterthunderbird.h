/*
  Copyright (c) 2011-2019 Montel Laurent <montel@kde.org>

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

#ifndef MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERTHUNDERBIRD_P_H
#define MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERTHUNDERBIRD_P_H

#include "filter/filterimporter/filterimporterabstract.h"
#include "mailcommon_export.h"
#include <QTextStream>

class QFile;

namespace MailCommon {
class MailFilter;
/**
 * @brief The FilterImporterThunderbird class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FilterImporterThunderbird : public FilterImporterAbstract
{
public:
    explicit FilterImporterThunderbird(QFile *file, bool interactive = true);
    FilterImporterThunderbird(QString string, bool interactive = true);
    ~FilterImporterThunderbird();
    static QString defaultThunderbirdFiltersSettingsPath();

    static QString defaultIcedoveFiltersSettingsPath();
    static QString defaultSeaMonkeyFiltersSettingsPath();
private:
    void readStream(QTextStream &stream);
    static QString cleanArgument(const QString &line, const QString &removeStr);
    void extractConditions(const QString &line, MailCommon::MailFilter *filter);
    QString extractActions(const QString &line, MailFilter *filter, QString &value);
    void extractType(const QString &line, MailCommon::MailFilter *filter);
    bool splitConditions(const QString &cond, MailCommon::MailFilter *filter);
    MailFilter *parseLine(QTextStream &stream, QString line, MailCommon::MailFilter *filter);
};
}

#endif /* FILTERIMPORTERTHUNDERBIRD_H */
