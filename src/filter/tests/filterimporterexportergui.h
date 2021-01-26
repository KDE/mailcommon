/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERIMPORTEREXPORTERGUI_H
#define FILTERIMPORTEREXPORTERGUI_H

#include "filter/filterimporterexporter.h"
#include <QWidget>
class QTextEdit;
class FilterImporterExporterGui : public QWidget
{
    Q_OBJECT
public:
    explicit FilterImporterExporterGui(QWidget *parent = nullptr);
    ~FilterImporterExporterGui();
private Q_SLOTS:
    void slotImportFilter(QAction *act);

private:
    void importFilters(MailCommon::FilterImporterExporter::FilterType type);
    QTextEdit *mTextEdit = nullptr;
};

#endif // FILTERIMPORTEREXPORTERGUI_H
