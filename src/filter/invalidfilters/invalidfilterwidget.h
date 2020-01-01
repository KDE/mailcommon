/*
  Copyright (c) 2015-2020 Laurent Montel <montel@kde.org>

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

#ifndef INVALIDFILTERWIDGET_H
#define INVALIDFILTERWIDGET_H

#include <QWidget>
#include "invalidfilterinfo.h"
#include "mailcommon_export.h"

namespace MailCommon {
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
    ~InvalidFilterWidget();

    void setInvalidFilters(const QVector<MailCommon::InvalidFilterInfo> &lst);
Q_SIGNALS:
    void showDetails(const QString &);
    void hideInformationWidget();
private:
    InvalidFilterListView *mInvalidFilterListWidget = nullptr;
};
}

#endif // INVALIDFILTERWIDGET_H
