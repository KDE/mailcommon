/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERPROCMAIL_P_H
#define MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERPROCMAIL_P_H

#include "filter/filterimporter/filterimporterabstract.h"

#include <QTextStream>

class QFile;

namespace MailCommon
{
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
