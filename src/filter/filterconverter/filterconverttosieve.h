/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QVector>

namespace MailCommon
{
class MailFilter;
class FilterConvertToSieve
{
public:
    FilterConvertToSieve(const QVector<MailFilter *> &filters);
    ~FilterConvertToSieve();

    void convert();

private:
    const QVector<MailFilter *> mListFilters;
};
}
