/*
  SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KMFILTERACCOUNTLIST_H
#define KMFILTERACCOUNTLIST_H

#include <QTreeWidget>

#include "mailcommon_private_export.h"

namespace MailCommon
{
class MailFilter;
class MAILCOMMON_TESTS_EXPORT KMFilterAccountList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit KMFilterAccountList(QWidget *parent);
    ~KMFilterAccountList() override;

    void updateAccountList(MailCommon::MailFilter *filter);
    void applyOnAccount(MailCommon::MailFilter *filter);
    void applyOnAccount(const QStringList &lst);

    QStringList selectedAccount();
};
}

#endif // KMFILTERACCOUNTLIST_H
