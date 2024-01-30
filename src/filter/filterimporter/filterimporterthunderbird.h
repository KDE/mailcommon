/*
  SPDX-FileCopyrightText: 2011-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "filter/filterimporter/filterimporterabstract.h"
#include "mailcommon_export.h"
#include <QTextStream>

class QFile;

namespace MailCommon
{
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
    [[nodiscard]] static QString defaultThunderbirdFiltersSettingsPath();

    [[nodiscard]] static QString defaultIcedoveFiltersSettingsPath();
    [[nodiscard]] static QString defaultSeaMonkeyFiltersSettingsPath();

private:
    MAILCOMMON_NO_EXPORT void readStream(QTextStream &stream);
    [[nodiscard]] static MAILCOMMON_NO_EXPORT QString cleanArgument(const QString &line, const QString &removeStr);
    MAILCOMMON_NO_EXPORT void extractConditions(const QString &line, MailCommon::MailFilter *filter);
    [[nodiscard]] MAILCOMMON_NO_EXPORT QString extractActions(const QString &line, MailFilter *filter, QString &value);
    MAILCOMMON_NO_EXPORT void extractType(const QString &line, MailCommon::MailFilter *filter);
    [[nodiscard]] MAILCOMMON_NO_EXPORT bool splitConditions(const QString &cond, MailCommon::MailFilter *filter);
    MAILCOMMON_NO_EXPORT MailFilter *parseLine(QTextStream &stream, QString line, MailCommon::MailFilter *filter);
};
}
