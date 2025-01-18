/*
  SPDX-FileCopyrightText: 2021 Intevation GmbH
  SPDX-FileContributor: Ingo Klöcker <dev@ingo-kloecker.de>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>
#include <vector>

class QAbstractItemModel;
class QModelIndex;
class QRegularExpression;

namespace MailCommon
{
/* This class allows filtering a hierarchical item model with path-like
 * patterns like "parent/sub" similar to file path globbing.
 *
 * The matching works as follows:
 * * The pattern is split into sub-patterns wherever "/" occurs.
 * * The sub-patterns are matched against each item and its ancestors.
 * * The pattern matches for an item if all sub-patterns match the item and
 *   its ancestors.
 * * The sub-pattern matching uses wildcard matching with implied "*" before
 *   and after each sub-pattern. For details see
 *   QRegularExpression::wildcardToRegularExpression().
 *
 * Examples:
 * * "foo" matches all items that contain "foo".
 * * "foo/bar" matches all items matching "bar" with parent items matching "foo".
 * * "foo/" matches all child items of items matching "foo".
 * * "/bar" matches all items matching "bar" which have a parent item.
 * * "foo//bar" matches all items matching "bar" with grandparents matching "foo".
 *
 * For more examples have a look at the tests in foldertreewidgettest.cpp.
 *
 * Design decisions:
 * * The original request was that "foo" should match items that contain "foo"
 *   *and* their (direct?) child items, so that one can easily narrow down the
 *   list of folders to the folders belonging to a subfolder tree, e.g. the
 *   subfolder tree for "project foo".
 *   I decided against this for the following reasons:
 *   * The requested result can be achieved with "foo/".
 *   * If there are multiple folders matching "foo" with a larger number of
 *     subfolders, then it would not have been easy to select the wanted
 *     folder matching "foo" because of all the subfolders that are also listed.
 *   * I also think that the behavior of the original request isn't really
 *     intuitive, at least for people who know file path globbing.
 */
class HierarchicalFolderMatcher
{
public:
    HierarchicalFolderMatcher();

    [[nodiscard]] bool isNull() const;

    void setFilter(const QString &filter, Qt::CaseSensitivity caseSensitivity);

    [[nodiscard]] bool matches(const QAbstractItemModel *model, const QModelIndex &start, int role = Qt::DisplayRole);

    [[nodiscard]] QModelIndex findFirstMatch(const QAbstractItemModel *model, const QModelIndex &start, int role = Qt::DisplayRole);

private:
    std::vector<QRegularExpression> filterRegExps;
};
}
