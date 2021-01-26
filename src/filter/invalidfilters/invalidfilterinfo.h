/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef INVALIDFILTERINFO_H
#define INVALIDFILTERINFO_H

#include "mailcommon_private_export.h"
#include <QString>
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT InvalidFilterInfo
{
public:
    InvalidFilterInfo();
    InvalidFilterInfo(const QString &name, const QString &information);

    Q_REQUIRED_RESULT QString information() const;

    Q_REQUIRED_RESULT QString name() const;

    void setName(const QString &name);

    void setInformation(const QString &information);

    Q_REQUIRED_RESULT bool operator==(const InvalidFilterInfo &other) const;

private:
    QString mName;
    QString mInformation;
};
}
Q_DECLARE_TYPEINFO(MailCommon::InvalidFilterInfo, Q_MOVABLE_TYPE);

#endif // INVALIDFILTERINFO_H
