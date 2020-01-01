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

#ifndef INVALIDFILTERINFO_H
#define INVALIDFILTERINFO_H

#include <QString>
#include "mailcommon_private_export.h"
namespace MailCommon {
class MAILCOMMON_TESTS_EXPORT InvalidFilterInfo
{
public:
    InvalidFilterInfo();
    InvalidFilterInfo(const QString &name, const QString &information);

    Q_REQUIRED_RESULT QString information() const;

    Q_REQUIRED_RESULT QString name() const;

    void setName(const QString &name);

    void setInformation(const QString &information);

    Q_REQUIRED_RESULT bool operator ==(const InvalidFilterInfo &other) const;
private:
    QString mName;
    QString mInformation;
};
}
Q_DECLARE_TYPEINFO(MailCommon::InvalidFilterInfo, Q_MOVABLE_TYPE);

#endif // INVALIDFILTERINFO_H
