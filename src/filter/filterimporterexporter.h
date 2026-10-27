/*
  SPDX-FileCopyrightText: 2007 Till Adam <adam@kde.org>
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

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
/*!
 * \class MailCommon::FilterImporterExporter
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FilterImporterExporter
 *
 * \brief Utility class that provides persisting of filters to/from KConfig.
 *
 * \author Till Adam <till@kdab.net>
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

    /*!
     * Creates a new filter importer/exporter.
     *
     * \a parent The parent widget.
     */
    explicit FilterImporterExporter(QWidget *parent = nullptr);

    /*!
     * Destroys the filter importer/exporter.
     */
    virtual ~FilterImporterExporter();

    /*!
     * Exports the given \a filters to the file \a fileName, or to a file which
     * is asked from the user if \a fileName is empty. Unless \a saveAll is
     * \c true, the list to export is also presented for confirmation/selection.
     */
    void exportFilters(const QList<MailFilter *> &filters, const QUrl &fileName = QUrl(), bool saveAll = false);

    /*!
     * Imports filters of the given \a type from the file \a filename. If
     * \a filename is empty, asks the user where to import them from.
     * The user is also asked which filters to import. \a canceled is set to
     * \c true if the user canceled the import.
     *
     * Returns the list of imported filters.
     */
    [[nodiscard]] QList<MailFilter *>
    importFilters(bool &canceled, FilterImporterExporter::FilterType type = FilterImporterExporter::KMailFilter, const QString &filename = QString());

    /*!
     * Writes the given list of \a filters to the given \a config file.
     * If \a exportFilter is \c true, the action arguments are written in their
     * exportable form.
     */
    static void writeFiltersToConfig(const QList<MailFilter *> &filters, KSharedConfig::Ptr config, bool exportFilter = false);

    /*!
     * Reads a list of filters from the given \a config file.
     * \a emptyFilters will contain filters that are empty after "purification", which removes aspects that are not applicable,
     * usually because of references to nonexistent external entities such as accounts, folders, etc.
     * If \a interactive is true, a dialog will offer to select a different account for any filter that refers to an account that doesn't exist.
     *
     * Returns the list of filters read.
     */
    static QList<MailFilter *> readFiltersFromConfig(const KSharedConfig::Ptr &config, bool interactive, QStringList &emptyFilters);

private:
    Q_DISABLE_COPY(FilterImporterExporter)

    class FilterImporterExporterPrivate;
    std::unique_ptr<FilterImporterExporterPrivate> const d;
};
}
