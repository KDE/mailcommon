/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef INVALIDFILTERDIALOG_H
#define INVALIDFILTERDIALOG_H

#include "invalidfilterinfo.h"
#include "mailcommon_private_export.h"
#include <QDialog>

namespace MailCommon
{
class InvalidFilterWidget;
class InvalidFilterInfoWidget;
class MAILCOMMON_TESTS_EXPORT InvalidFilterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InvalidFilterDialog(QWidget *parent = nullptr);
    ~InvalidFilterDialog();

    void setInvalidFilters(const QVector<InvalidFilterInfo> &lst);

private:
    void writeConfig();
    void readConfig();
    InvalidFilterWidget *mInvalidFilterWidget = nullptr;
    InvalidFilterInfoWidget *mInvalidFilterInfoWidget = nullptr;
};
}
#endif // INVALIDFILTERDIALOG_H
