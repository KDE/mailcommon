/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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
    ~InvalidFilterDialog() override;

    void setInvalidFilters(const QList<InvalidFilterInfo> &lst);

private:
    MAILCOMMON_NO_EXPORT void writeConfig();
    MAILCOMMON_NO_EXPORT void readConfig();
    InvalidFilterWidget *const mInvalidFilterWidget;
    InvalidFilterInfoWidget *const mInvalidFilterInfoWidget;
};
}
