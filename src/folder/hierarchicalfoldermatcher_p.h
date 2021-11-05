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

    Q_REQUIRED_RESULT bool isNull() const;

    void setFilter(const QString &filter, Qt::CaseSensitivity caseSensitivity);

    Q_REQUIRED_RESULT bool matches(const QAbstractItemModel *model, const QModelIndex &start, int role = Qt::DisplayRole);

    Q_REQUIRED_RESULT QModelIndex findFirstMatch(const QAbstractItemModel *model, const QModelIndex &start, int role = Qt::DisplayRole);

private:
    std::vector<QRegularExpression> filterRegExps;
};
}