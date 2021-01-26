/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERCLAWSMAILS_P_H
#define MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERCLAWSMAILS_P_H

#include "filterimporterabstract.h"
#include "mailcommon_export.h"

class QFile;
class QTextStream;

namespace MailCommon
{
class MailFilter;

class MAILCOMMON_EXPORT FilterImporterClawsMails : public FilterImporterAbstract
{
public:
    explicit FilterImporterClawsMails(QFile *file);
    FilterImporterClawsMails(QString string);
    // Use for unittests
    FilterImporterClawsMails(bool interactive = false);
    ~FilterImporterClawsMails();
    static QString defaultFiltersSettingsPath();

    // the returned mail filter instance will be owned by the caller, who must ensure to delete it at some point
    Q_REQUIRED_RESULT MailFilter *parseLine(const QString &line);

private:
    QString extractString(const QString &tmp, int &pos);
    QString extractConditions(const QString &line, MailFilter *filter);
    QString extractActions(const QString &line, MailFilter *filter);
    void readStream(QTextStream &stream);
};
}

#endif // MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERCLAWSMAILS_P_H
