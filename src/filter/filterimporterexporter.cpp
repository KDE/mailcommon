/*
  SPDX-FileCopyrightText: 2007 Till Adam <adam@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterimporterexporter.h"
#include "dialog/selectthunderbirdfilterfilesdialog.h"
#include "filteractions/filteraction.h"
#include "filterimporter/filterimporterbalsa.h"
#include "filterimporter/filterimporterclawsmail.h"
#include "filterimporter/filterimporterevolution.h"
#include "filterimporter/filterimportergmail.h"
#include "filterimporter/filterimporterprocmail.h"
#include "filterimporter/filterimportersylpheed.h"
#include "filterimporter/filterimporterthunderbird.h"
#include "filtermanager.h"
#include "filterselectiondialog.h"
#include "mailcommon_debug.h"
#include "mailfilter.h"

#include <MessageViewer/MessageViewerUtil>

#include <KConfig>
#include <KConfigGroup>
#include <KListWidgetSearchLine>
#include <KMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QRegularExpression>

using namespace MailCommon;

QVector<MailFilter *> FilterImporterExporter::readFiltersFromConfig(const KSharedConfig::Ptr &config, QStringList &emptyFilters)
{
    const KConfigGroup group = config->group("General");

    const int numFilters = group.readEntry("filters", 0);

    bool filterNeedUpdate = false;
    QVector<MailFilter *> filters;
    for (int i = 0; i < numFilters; ++i) {
        const QString groupName = QStringLiteral("Filter #%1").arg(i);

        const KConfigGroup group = config->group(groupName);
        bool update = false;
        auto filter = new MailFilter(group, true /*interactive*/, update);
        filter->purify();
        if (update) {
            filterNeedUpdate = true;
        }
        if (filter->isEmpty()) {
#ifndef NDEBUG
            qCDebug(MAILCOMMON_LOG) << "Filter" << filter->asString() << "is empty!";
#endif
            emptyFilters << filter->name();
            delete filter;
        } else {
            filters.append(filter);
        }
    }
    if (filterNeedUpdate) {
        KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("akonadi_mailfilter_agentrc"));

        // Now, write out the new stuff:
        FilterImporterExporter::writeFiltersToConfig(filters, config);
        KConfigGroup group = config->group("General");
        group.sync();
    }
    return filters;
}

void FilterImporterExporter::writeFiltersToConfig(const QVector<MailFilter *> &filters, KSharedConfig::Ptr config, bool exportFiler)
{
    // first, delete all filter groups:
    const QStringList filterGroups = config->groupList().filter(QRegularExpression(QStringLiteral("Filter #\\d+")));

    for (const QString &group : filterGroups) {
        config->deleteGroup(group);
    }

    int i = 0;
    for (const MailFilter *filter : filters) {
        if (!filter->isEmpty()) {
            const QString groupName = QStringLiteral("Filter #%1").arg(i);

            KConfigGroup group = config->group(groupName);
            filter->writeConfig(group, exportFiler);
            ++i;
        }
    }

    KConfigGroup group = config->group("General");
    group.writeEntry("filters", i);

    config->sync();
}

class Q_DECL_HIDDEN FilterImporterExporter::Private
{
public:
    Private(QWidget *parent)
        : mParent(parent)
    {
    }

    void warningInfoAboutInvalidFilter(const QStringList &emptyFilters) const;
    QWidget *mParent = nullptr;
};

void FilterImporterExporter::Private::warningInfoAboutInvalidFilter(const QStringList &emptyFilters) const
{
    if (!emptyFilters.isEmpty()) {
        KMessageBox::informationList(mParent,
                                     i18n("The following filters have not been saved because they were invalid "
                                          "(e.g. containing no actions or no search rules)."),
                                     emptyFilters,
                                     QString(),
                                     QStringLiteral("ShowInvalidFilterWarning"));
    }
}

FilterImporterExporter::FilterImporterExporter(QWidget *parent)
    : d(new Private(parent))
{
}

FilterImporterExporter::~FilterImporterExporter()
{
    delete d;
}

QVector<MailFilter *> FilterImporterExporter::importFilters(bool &canceled, FilterImporterExporter::FilterType type, const QString &filename)
{
    QString fileName(filename);

    QFile file;
    if ((type != ThunderBirdFilter) && (type != IcedoveFilter) && (type != SeaMonkeyFilter)) {
        if (fileName.isEmpty()) {
            QString title;
            QString defaultPath;
            switch (type) {
            case KMailFilter:
                title = i18n("Import KMail Filters");
                defaultPath = QDir::homePath();
                break;
            case ThunderBirdFilter:
            case IcedoveFilter:
            case SeaMonkeyFilter:
                break;
            case EvolutionFilter:
                title = i18n("Import Evolution Filters");
                defaultPath = MailCommon::FilterImporterEvolution::defaultFiltersSettingsPath();
                break;
            case SylpheedFilter:
                title = i18n("Import Sylpheed Filters");
                defaultPath = MailCommon::FilterImporterSylpheed::defaultFiltersSettingsPath();
                break;
            case ProcmailFilter:
                title = i18n("Import Procmail Filters");
                defaultPath = MailCommon::FilterImporterProcmail::defaultFiltersSettingsPath();
                break;
            case BalsaFilter:
                title = i18n("Import Balsa Filters");
                defaultPath = MailCommon::FilterImporterBalsa::defaultFiltersSettingsPath();
                break;
            case ClawsMailFilter:
                title = i18n("Import Claws Mail Filters");
                defaultPath = MailCommon::FilterImporterClawsMails::defaultFiltersSettingsPath();
                break;
            case GmailFilter:
                title = i18n("Import Gmail Filters");
                defaultPath = MailCommon::FilterImporterGmail::defaultFiltersSettingsPath();
                break;
            }

            fileName = QFileDialog::getOpenFileName(d->mParent, title, defaultPath);
            if (fileName.isEmpty()) {
                canceled = true;
                return QVector<MailFilter *>(); // cancel
            }
        }
        file.setFileName(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            KMessageBox::error(d->mParent,
                               i18n("The selected file is not readable. "
                                    "Your file access permissions might be insufficient."));
            return QVector<MailFilter *>();
        }
    }

    QVector<MailFilter *> imported;
    QStringList emptyFilter;

    switch (type) {
    case KMailFilter: {
        const KSharedConfig::Ptr config = KSharedConfig::openConfig(fileName);
        imported = readFiltersFromConfig(config, emptyFilter);
        break;
    }
    case IcedoveFilter:
    case SeaMonkeyFilter:
    case ThunderBirdFilter:
        if (fileName.isEmpty()) {
            QString defaultPath;
            if (type == ThunderBirdFilter) {
                defaultPath = MailCommon::FilterImporterThunderbird::defaultThunderbirdFiltersSettingsPath();
            } else if (type == IcedoveFilter) {
                defaultPath = MailCommon::FilterImporterThunderbird::defaultIcedoveFiltersSettingsPath();
            } else if (type == SeaMonkeyFilter) {
                defaultPath = MailCommon::FilterImporterThunderbird::defaultSeaMonkeyFiltersSettingsPath();
            }

            QPointer<SelectThunderbirdFilterFilesDialog> selectThunderBirdFileDialog = new SelectThunderbirdFilterFilesDialog(defaultPath, d->mParent);
            selectThunderBirdFileDialog->setStartDir(QUrl::fromLocalFile(defaultPath));
            if (selectThunderBirdFileDialog->exec()) {
                const QStringList lstFiles = selectThunderBirdFileDialog->selectedFiles();
                for (const QString &url : lstFiles) {
                    QFile fileThunderbird(url);
                    if (!fileThunderbird.open(QIODevice::ReadOnly)) {
                        KMessageBox::error(d->mParent,
                                           i18n("The selected file is not readable. "
                                                "Your file access permissions might be insufficient."));
                    } else {
                        auto *thunderBirdFilter = new MailCommon::FilterImporterThunderbird(&fileThunderbird);

                        imported.append(thunderBirdFilter->importFilter());
                        emptyFilter.append(thunderBirdFilter->emptyFilter());
                        delete thunderBirdFilter;
                    }
                }
            } else {
                canceled = true;
                delete selectThunderBirdFileDialog;
                return QVector<MailFilter *>();
            }
            delete selectThunderBirdFileDialog;
        } else {
            file.setFileName(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                KMessageBox::error(d->mParent,
                                   i18n("The selected file is not readable. "
                                        "Your file access permissions might be insufficient."));
                return QVector<MailFilter *>();
            }

            auto thunderBirdFilter = new MailCommon::FilterImporterThunderbird(&file);
            imported = thunderBirdFilter->importFilter();
            emptyFilter = thunderBirdFilter->emptyFilter();
            delete thunderBirdFilter;
        }
        break;
    case EvolutionFilter: {
        auto *filter = new MailCommon::FilterImporterEvolution(&file);

        imported = filter->importFilter();
        emptyFilter = filter->emptyFilter();
        delete filter;
        break;
    }
    case SylpheedFilter: {
        auto *filter = new MailCommon::FilterImporterSylpheed(&file);

        imported = filter->importFilter();
        emptyFilter = filter->emptyFilter();
        delete filter;
        break;
    }
    case ProcmailFilter: {
        auto *filter = new MailCommon::FilterImporterProcmail(&file);

        imported = filter->importFilter();
        emptyFilter = filter->emptyFilter();
        delete filter;
        break;
    }
    case BalsaFilter: {
        auto *filter = new MailCommon::FilterImporterBalsa(&file);

        imported = filter->importFilter();
        emptyFilter = filter->emptyFilter();
        delete filter;
        break;
    }
    case ClawsMailFilter: {
        auto *filter = new MailCommon::FilterImporterClawsMails(&file);

        imported = filter->importFilter();
        emptyFilter = filter->emptyFilter();
        delete filter;
        break;
    }
    case GmailFilter: {
        auto *filter = new MailCommon::FilterImporterGmail(&file);

        imported = filter->importFilter();
        emptyFilter = filter->emptyFilter();
        delete filter;
        break;
    }
    }
    d->warningInfoAboutInvalidFilter(emptyFilter);
    file.close();

    QPointer<FilterSelectionDialog> dlg = new FilterSelectionDialog(d->mParent);
    dlg->setFilters(imported);
    if (dlg->exec() == QDialog::Accepted) {
        const QVector<MailFilter *> selected = dlg->selectedFilters();
        delete dlg;
        return selected;
    }
    delete dlg;
    canceled = true;
    return QVector<MailFilter *>();
}

void FilterImporterExporter::exportFilters(const QVector<MailFilter *> &filters, const QUrl &fileName, bool saveAll)
{
    QUrl saveUrl;
    if (fileName.isEmpty()) {
        saveUrl = QFileDialog::getSaveFileUrl(d->mParent,
                                              i18n("Export Filters"),
                                              QUrl::fromLocalFile(QDir::homePath()),
                                              QString(),
                                              nullptr,
                                              QFileDialog::DontConfirmOverwrite);

        if (saveUrl.isEmpty() || !MessageViewer::Util::checkOverwrite(saveUrl, d->mParent)) {
            qDeleteAll(filters);
            return;
        }
    } else {
        saveUrl = fileName;
    }
    KSharedConfig::Ptr config = KSharedConfig::openConfig(saveUrl.toLocalFile());
    if (saveAll) {
        writeFiltersToConfig(filters, config, true);
        // qDeleteAll(filters);
    } else {
        std::unique_ptr<FilterSelectionDialog> dlg(new FilterSelectionDialog(d->mParent));
        dlg->setFilters(filters);
        if (dlg->exec() == QDialog::Accepted && dlg) {
            QVector<MailFilter *> lst = dlg->selectedFilters();
            writeFiltersToConfig(lst, config, true);
            qDeleteAll(lst);
        }
    }
}
