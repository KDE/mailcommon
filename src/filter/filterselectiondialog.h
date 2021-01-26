/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FILTERSELECTIONDIALOG_H
#define MAILCOMMON_FILTERSELECTIONDIALOG_H

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
    QVector<MailFilter *> selectedFilters() const;

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
#endif // FILTERSELECTIONDIALOG_H
