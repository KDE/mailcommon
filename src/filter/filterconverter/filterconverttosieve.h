/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

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
    explicit FilterConvertToSieve(const QList<MailFilter *> &filters);
    ~FilterConvertToSieve();

    void convert();

private:
    const QList<MailFilter *> mListFilters;
};
}
