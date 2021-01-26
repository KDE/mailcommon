/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FILTERIMPORTER_FILTERIMPORTEREVOLUTION_P_H
#define MAILCOMMON_FILTERIMPORTER_FILTERIMPORTEREVOLUTION_P_H

#include "filter/filterimporter/filterimporterabstract.h"

#include <QDomElement>

class QFile;

namespace MailCommon
{
class MailFilter;

class FilterImporterEvolution : public FilterImporterAbstract
{
public:
    explicit FilterImporterEvolution(QFile *file);
    ~FilterImporterEvolution();
    static QString defaultFiltersSettingsPath();

private:
    enum parseType { PartType = 0, ActionType = 1 };
    void parseFilters(const QDomElement &e);
    void parsePartAction(const QDomElement &ruleFilter, MailCommon::MailFilter *filter, MailCommon::FilterImporterEvolution::parseType type);
};
}

#endif // FILTERIMPORTEREVOLUTION_H
