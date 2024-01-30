/*
  SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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
    [[nodiscard]] static QString defaultFiltersSettingsPath();

private:
    enum parseType {
        PartType = 0,
        ActionType = 1,
    };
    void parseFilters(const QDomElement &e);
    void parsePartAction(const QDomElement &ruleFilter, MailCommon::MailFilter *filter, MailCommon::FilterImporterEvolution::parseType type);
};
}
