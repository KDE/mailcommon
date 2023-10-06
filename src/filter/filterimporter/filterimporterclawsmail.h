/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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
    [[nodiscard]] MailFilter *parseLine(const QString &line);

private:
    [[nodiscard]] MAILCOMMON_NO_EXPORT QString extractString(const QString &tmp, int &pos);
    [[nodiscard]] MAILCOMMON_NO_EXPORT QString extractConditions(const QString &line, MailFilter *filter);
    [[nodiscard]] MAILCOMMON_NO_EXPORT QString extractActions(const QString &line, MailFilter *filter);
    MAILCOMMON_NO_EXPORT void readStream(QTextStream &stream);
};
}
