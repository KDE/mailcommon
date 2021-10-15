/*
  SPDX-FileCopyrightText: 2021 Intevation GmbH
  SPDX-FileContributor: Ingo Klöcker <dev@ingo-kloecker.de>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <Qt>

#include <vector>

class QAbstractItemModel;
class QModelIndex;
class QRegularExpression;
class QString;

namespace MailCommon
{
class HierarchicalFolderMatcher
{
public:
    HierarchicalFolderMatcher();

    bool isNull();

    void setFilter(const QString &filter, Qt::CaseSensitivity caseSensitivity);

    bool matches(const QAbstractItemModel *model, const QModelIndex &start, int role = Qt::DisplayRole);

    QModelIndex findFirstMatch(const QAbstractItemModel *model, const QModelIndex &start, int role = Qt::DisplayRole);

private:
    std::vector<QRegularExpression> filterRegExps;
};
}
