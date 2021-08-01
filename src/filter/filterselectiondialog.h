/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include <QVector>

class QPushButton;

class QListWidget;
class QWidget;

namespace MailCommon
{
class MailFilter;

class FilterSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterSelectionDialog(QWidget *parent = nullptr);
    ~FilterSelectionDialog() override;
    void setFilters(const QVector<MailFilter *> &filters);
    Q_REQUIRED_RESULT QVector<MailFilter *> selectedFilters() const;

public Q_SLOTS:
    void slotUnselectAllButton();
    void slotSelectAllButton();
    void reject() override;

private:
    void writeConfig();
    void readConfig();
    QListWidget *filtersListWidget = nullptr;
    QVector<MailFilter *> originalFilters;
    QPushButton *selectAllButton = nullptr;
    QPushButton *unselectAllButton = nullptr;
    QPushButton *mOkButton = nullptr;
};
}
