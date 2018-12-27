/*
  Copyright (c) 2015-2019 Montel Laurent <montel@kde.org>

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

#ifndef INVALIDFILTERDIALOG_H
#define INVALIDFILTERDIALOG_H

#include "mailcommon_private_export.h"
#include "invalidfilterinfo.h"
#include <QDialog>

namespace MailCommon {
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
