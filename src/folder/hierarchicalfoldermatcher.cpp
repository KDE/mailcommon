/*
  SPDX-FileCopyrightText: 2021 Intevation GmbH
  SPDX-FileContributor: Ingo Klöcker <dev@ingo-kloecker.de>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "hierarchicalfoldermatcher_p.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QRegularExpression>

namespace MailCommon
{

HierarchicalFolderMatcher::HierarchicalFolderMatcher()
{
}

bool HierarchicalFolderMatcher::isNull() const
{
    return filterRegExps.empty();
}

void HierarchicalFolderMatcher::setFilter(const QString &filter, Qt::CaseSensitivity caseSensitivity)
{
    filterRegExps.clear();
    if (filter.isEmpty()) {
        return;
    }
    const auto patternOptions = caseSensitivity == Qt::CaseInsensitive ?
        QRegularExpression::CaseInsensitiveOption :
        QRegularExpression::NoPatternOption;
    const auto parts = filter.split(QLatin1Char('/'));
    std::transform(std::begin(parts), std::end(parts),
                    std::back_inserter(filterRegExps),
                    [patternOptions](const auto &part) {
                        // QRegularExpression::wildcardToRegularExpression() returns a fully anchored
                        // regular expression, but we want to check for substring matches; wrap
                        // the user's filter part into '*' to fix this
                        return QRegularExpression{QRegularExpression::wildcardToRegularExpression(
                            QLatin1Char('*') + part + QLatin1Char('*')), patternOptions};
                    });
}

bool HierarchicalFolderMatcher::matches(const QAbstractItemModel *model, const QModelIndex &start, int role)
{
    if (!start.isValid()) {
        return false;
    }

    const auto filterKeyColumn = start.column();
    QModelIndex idx = start;
    for (auto it = filterRegExps.crbegin(); it != filterRegExps.crend(); ++it) {
        if (!idx.isValid()) {
            // we have exceeded the model root or the column does not exist
            return false;
        }
        const QString key = model->data(idx, role).toString();
        if (!it->match(key).hasMatch()) {
            return false;
        }
        idx = idx.parent().siblingAtColumn(filterKeyColumn);
    }
    return true;
}

QModelIndex HierarchicalFolderMatcher::findFirstMatch(const QAbstractItemModel *model, const QModelIndex &start, int role)
{
    // inspired by QAbstractItemModel::match(), but using our own matching
    QModelIndex result;

    const int filterKeyColumn = start.column();
    const QModelIndex p = model->parent(start);
    int from = start.row();
    int to = model->rowCount(p);

    // iterate twice (first from start row to last row; then from first row to before start row)
    for (int i = 0; (i < 2) && !result.isValid(); ++i) {
        for (int row = from; (row < to) && !result.isValid(); ++row) {
            QModelIndex idx = model->index(row, filterKeyColumn, p);
            if (!idx.isValid()) {
                continue;
            }
            if (matches(model, idx, role)) {
                result = idx;
                break;
            }
            const auto idxAsParent = filterKeyColumn != 0 ? idx.siblingAtColumn(0) : idx;
            if (model->hasChildren(idxAsParent)) {
                result = findFirstMatch(model, model->index(0, filterKeyColumn, idxAsParent), role);
            }
        }
        // prepare for the next iteration
        from = 0;
        to = start.row();
    }

    return result;
}

}
