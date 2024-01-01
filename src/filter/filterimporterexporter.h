/*
  SPDX-FileCopyrightText: 2007 Till Adam <adam@kde.org>
  SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"

#include <KSharedConfig>
#include <QUrl>

#include <QList>
#include <QStringList>
#include <memory>
class QWidget;

namespace MailCommon
{
class MailFilter;

/**
 * @short Utility class that provides persisting of filters to/from KConfig.
 *
 * @author Till Adam <till@kdab.net>
 */
class MAILCOMMON_EXPORT FilterImporterExporter
{
public:
    enum FilterType {
        KMailFilter = 0,
        ThunderBirdFilter = 1,
        EvolutionFilter = 2,
        SylpheedFilter = 3,
        ProcmailFilter = 4,
        BalsaFilter = 5,
        ClawsMailFilter = 6,
        IcedoveFilter = 7,
        GmailFilter = 8,
        SeaMonkeyFilter = 9,
    };

    /**
     * Creates a new filter importer/exporter.
     *
     * @param parent The parent widget.
     */
    explicit FilterImporterExporter(QWidget *parent = nullptr);

    /**
     * Destroys the filter importer/exporter.
     */
    virtual ~FilterImporterExporter();

    /**
     * Exports the given @p filters to a file which
     * is asked from the user. The list to export is also
     * presented for confirmation/selection.
     */
    void exportFilters(const QList<MailFilter *> &filters, const QUrl &fileName = QUrl(), bool saveAll = false);

    /**
     * Imports filters. Ask the user where to import them from
     * and which filters to import.
     */
    [[nodiscard]] QList<MailFilter *>
    importFilters(bool &canceled, FilterImporterExporter::FilterType type = FilterImporterExporter::KMailFilter, const QString &filename = QString());

    /**
     * Writes the given list of @p filters to the given @p config file.
     */
    static void writeFiltersToConfig(const QList<MailFilter *> &filters, KSharedConfig::Ptr config, bool exportFilter = false);

    /**
     * Reads a list of filters from the given @p config file.
     * Return list of empty filter
     */
    static QList<MailFilter *> readFiltersFromConfig(const KSharedConfig::Ptr &config, QStringList &emptyFilter);

private:
    //@cond PRIVATE
    Q_DISABLE_COPY(FilterImporterExporter)

    class FilterImporterExporterPrivate;
    std::unique_ptr<FilterImporterExporterPrivate> const d;
    //@endcond
};
}
