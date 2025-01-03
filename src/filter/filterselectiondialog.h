/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include <QList>

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
    void setFilters(const QList<MailFilter *> &filters);
    [[nodiscard]] QList<MailFilter *> selectedFilters() const;

public Q_SLOTS:
    void slotUnselectAllButton();
    void slotSelectAllButton();
    void reject() override;

private:
    void writeConfig();
    void readConfig();
    QListWidget *const filtersListWidget;
    QList<MailFilter *> originalFilters;
    QPushButton *const selectAllButton;
    QPushButton *const unselectAllButton;
    QPushButton *mOkButton = nullptr;
};
}
