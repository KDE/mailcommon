/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "filter/filterimporterexporter.h"
#include <QWidget>
class QTextEdit;
class FilterImporterExporterGui : public QWidget
{
    Q_OBJECT
public:
    explicit FilterImporterExporterGui(QWidget *parent = nullptr);
    ~FilterImporterExporterGui() override;
private Q_SLOTS:
    void slotImportFilter(QAction *act);

private:
    void importFilters(MailCommon::FilterImporterExporter::FilterType type);
    QTextEdit *mTextEdit = nullptr;
};
