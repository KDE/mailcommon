/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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
    [[nodiscard]] static QString defaultFiltersSettingsPath();

private:
    [[nodiscard]] MailCommon::MailFilter *parseLine(QTextStream &stream, QString line, MailCommon::MailFilter *filter);
    void readStream(QTextStream &stream);
    [[nodiscard]] QString createUniqFilterName();
    int mFilterCount = 0;
};
}
