/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QList>

namespace MailCommon
{
class MailFilter;
class FilterConvertToSieve
{
public:
    FilterConvertToSieve(const QList<MailFilter *> &filters);
    ~FilterConvertToSieve();

    void convert();

private:
    const QList<MailFilter *> mListFilters;
};
}
