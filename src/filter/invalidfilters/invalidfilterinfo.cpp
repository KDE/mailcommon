/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "invalidfilterinfo.h"
using namespace MailCommon;

InvalidFilterInfo::InvalidFilterInfo()
{
}

InvalidFilterInfo::InvalidFilterInfo(const QString &name, const QString &information)
    : mName(name)
    , mInformation(information)
{
}

QString InvalidFilterInfo::information() const
{
    return mInformation;
}

QString InvalidFilterInfo::name() const
{
    return mName;
}

void InvalidFilterInfo::setName(const QString &name)
{
    mName = name;
}

void InvalidFilterInfo::setInformation(const QString &information)
{
    mInformation = information;
}

bool InvalidFilterInfo::operator==(const InvalidFilterInfo &other) const
{
    return (mName == other.name()) && (mInformation == other.information());
}
