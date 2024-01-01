/*
  SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "invalidfilterinfo.h"
#include "mailcommon_export.h"
#include <QWidget>

namespace MailCommon
{
class InvalidFilterListView;
/**
 * @brief The InvalidFilterWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT InvalidFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InvalidFilterWidget(QWidget *parent = nullptr);
    ~InvalidFilterWidget() override;

    void setInvalidFilters(const QList<MailCommon::InvalidFilterInfo> &lst);
Q_SIGNALS:
    void showDetails(const QString &);
    void hideInformationWidget();

private:
    InvalidFilterListView *const mInvalidFilterListWidget;
};
}
