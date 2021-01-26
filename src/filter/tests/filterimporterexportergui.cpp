/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filterimporterexportergui.h"
#include "filter/mailfilter.h"

#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QTextEdit>
#include <QVBoxLayout>

Q_DECLARE_METATYPE(MailCommon::FilterImporterExporter::FilterType)
FilterImporterExporterGui::FilterImporterExporterGui(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    auto menuBar = new QMenuBar(this);
    mainLayout->addWidget(menuBar);
    QMenu *menuFilter = menuBar->addMenu(QStringLiteral("filter"));
    auto act = new QAction(QStringLiteral("KMail filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::KMailFilter));
    menuFilter->addAction(act);

    act = new QAction(QStringLiteral("Thunderbird filters"), this);

    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::ThunderBirdFilter));
    menuFilter->addAction(act);

    act = new QAction(QStringLiteral("Evolution filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::EvolutionFilter));
    menuFilter->addAction(act);

    act = new QAction(QStringLiteral("Sylpheed filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::SylpheedFilter));
    menuFilter->addAction(act);

    act = new QAction(QStringLiteral("Procmail filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::ProcmailFilter));
    menuFilter->addAction(act);

    act = new QAction(QStringLiteral("Balsa filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::BalsaFilter));
    menuFilter->addAction(act);

    act = new QAction(QStringLiteral("Claws Mail filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::ClawsMailFilter));
    menuFilter->addAction(act);

    act = new QAction(QStringLiteral("Icedove Mail filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::IcedoveFilter));
    menuFilter->addAction(act);
    act = new QAction(QStringLiteral("GMail filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::GmailFilter));
    menuFilter->addAction(act);
    connect(menuFilter, QOverload<QAction *>::of(&QMenu::triggered), this, &FilterImporterExporterGui::slotImportFilter);

    mTextEdit = new QTextEdit;
    mTextEdit->setReadOnly(true);
    mainLayout->addWidget(mTextEdit);
}

FilterImporterExporterGui::~FilterImporterExporterGui()
{
}

void FilterImporterExporterGui::slotImportFilter(QAction *act)
{
    if (act) {
        importFilters(act->data().value<MailCommon::FilterImporterExporter::FilterType>());
    }
}

void FilterImporterExporterGui::importFilters(MailCommon::FilterImporterExporter::FilterType type)
{
    MailCommon::FilterImporterExporter importer(this);
    bool canceled = false;
    const QVector<MailCommon::MailFilter *> filters = importer.importFilters(canceled, type);
    if (canceled) {
        mTextEdit->setText(QStringLiteral("Canceled"));
        return;
    }
    QString result;
    for (MailCommon::MailFilter *filter : filters) {
        if (!result.isEmpty()) {
            result += QLatin1Char('\n');
        }
        result += filter->asString();
    }
    mTextEdit->setText(result);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    auto w = new FilterImporterExporterGui();
    w->resize(800, 600);
    w->show();
    app.exec();
    delete w;
    return 0;
}
