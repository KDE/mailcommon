/*
  Copyright (c) 2014-2020 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef KMFILTERACCOUNTLIST_H
#define KMFILTERACCOUNTLIST_H

#include <QTreeWidget>

#include "mailcommon_private_export.h"

namespace MailCommon {
class MailFilter;
class MAILCOMMON_TESTS_EXPORT KMFilterAccountList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit KMFilterAccountList(QWidget *parent);
    ~KMFilterAccountList();

    void updateAccountList(MailCommon::MailFilter *filter);
    void applyOnAccount(MailCommon::MailFilter *filter);
    void applyOnAccount(const QStringList &lst);

    QStringList selectedAccount();
};
}

#endif // KMFILTERACCOUNTLIST_H
