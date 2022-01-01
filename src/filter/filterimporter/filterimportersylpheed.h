/*
  SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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

