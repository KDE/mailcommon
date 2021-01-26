/*
  SPDX-FileCopyrightText: 2011-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERTHUNDERBIRD_P_H
#define MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERTHUNDERBIRD_P_H

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
