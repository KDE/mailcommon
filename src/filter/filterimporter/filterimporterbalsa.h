/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "filterimporterabstract.h"

class QFile;
class KConfigGroup;
class KConfig;

namespace MailCommon
{
class MailFilter;

class FilterImporterBalsa : public FilterImporterAbstract
{
public:
    explicit FilterImporterBalsa(QFile *file);
    FilterImporterBalsa();
    ~FilterImporterBalsa();
    static QString defaultFiltersSettingsPath();
    void readConfig(KConfig *config);

private:
    void parseFilter(const KConfigGroup &grp);
    void parseCondition(const QString &condition, MailCommon::MailFilter *filter);
    void parseAction(int actionType, const QString &action, MailCommon::MailFilter *filter);
};
}

