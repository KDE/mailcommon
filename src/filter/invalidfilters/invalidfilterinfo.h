/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <QString>
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT InvalidFilterInfo
{
public:
    InvalidFilterInfo();
    InvalidFilterInfo(const QString &name, const QString &information);

    [[nodiscard]] QString information() const;

    [[nodiscard]] QString name() const;

    void setName(const QString &name);

    void setInformation(const QString &information);

    [[nodiscard]] bool operator==(const InvalidFilterInfo &other) const;

private:
    QString mName;
    QString mInformation;
};
}
Q_DECLARE_TYPEINFO(MailCommon::InvalidFilterInfo, Q_RELOCATABLE_TYPE);
