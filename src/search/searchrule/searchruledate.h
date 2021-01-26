/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SEARCHRULEDATE_H
#define SEARCHRULEDATE_H

#include "searchpattern.h"
#include <AkonadiCore/Item>
namespace MailCommon
{
class SearchRuleDate : public SearchRule
{
public:
    /**
     * Creates new date search rule.
     *
     * @param field The field to search in.
     * @param function The function to use for searching.
     * @param contents The contents to search for.
     */
    explicit SearchRuleDate(const QByteArray &field = QByteArray(), Function function = FuncContains, const QString &contents = QString());
    /**
     * @copydoc SearchRule::isEmpty()
     */
    bool isEmpty() const override;

    /**
     * @copydoc SearchRule::matches()
     */
    bool matches(const Akonadi::Item &item) const override;

    /**
     * @copydoc SearchRule::requiredPart()
     */
    RequiredPart requiredPart() const override;

    // Optimized matching not implemented, will use the unoptimized matching
    // from SearchRule
    using SearchRule::matches;

    /**
     * A helper method for the main matches() method.
     * Does the actual comparing.
     */
    bool matchesInternal(QDate dateValue, QDate msgDate) const;

    /**
     * @copydoc SearchRule::addQueryTerms()
     */
    void addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const override;
    QString informationAboutNotValidRules() const override;
};
}

#endif // SEARCHRULEDATE_H
