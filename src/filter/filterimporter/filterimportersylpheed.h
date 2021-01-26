/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERSYLPHEED_P_H
#define MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERSYLPHEED_P_H

#include "filter/filterimporter/filterimporterabstract.h"

#include <QDomElement>

class QFile;

namespace MailCommon
{
class MailFilter;

class FilterImporterSylpheed : public FilterImporterAbstract
{
public:
    explicit FilterImporterSylpheed(QFile *file);
    ~FilterImporterSylpheed();
    static QString defaultFiltersSettingsPath();

private:
    void parseFilters(const QDomElement &e);
    void parseConditions(const QDomElement &ruleFilter, MailCommon::MailFilter *filter);
    void parseActions(const QDomElement &e, MailCommon::MailFilter *filter);
};
}

#endif /* FILTERIMPORTERSYLPHEED_P_H */
