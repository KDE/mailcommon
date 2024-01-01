/*
  SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "invalidfilterinfo.h"
#include "mailcommon_private_export.h"
#include <QListView>

namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT InvalidFilterListView : public QListView
{
    Q_OBJECT
public:
    explicit InvalidFilterListView(QWidget *parent = nullptr);
    ~InvalidFilterListView() override;
    void setInvalidFilters(const QList<MailCommon::InvalidFilterInfo> &lst);
Q_SIGNALS:
    void showDetails(const QString &);
    void hideInformationWidget();
};
}
