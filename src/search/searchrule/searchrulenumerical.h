/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SEARCHRULENUMERICAL_H
#define SEARCHRULENUMERICAL_H

#include "searchpattern.h"
#include <AkonadiCore/Item>
namespace MailCommon
{
/**
 * @short This class represents a search pattern rule operating on numerical values.
 *
 * This class represents a search to be performed against a numerical value,
 * such as the age of the message in days or its size.
 */
class SearchRuleNumerical : public SearchRule
{
public:
    /**
     * Creates new numerical search rule.
     *
     * @param field The field to search in.
     * @param function The function to use for searching.
     * @param contents The contents to search for.
     */
    explicit SearchRuleNumerical(const QByteArray &field = QByteArray(), Function function = FuncContains, const QString &contents = QString());

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
    bool matchesInternal(long numericalValue, long numericalContents, const QString &contents) const;

    /**
     * @copydoc SearchRule::addQueryTerms()
     */
    void addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const override;
    QString informationAboutNotValidRules() const override;
};
}
#endif // SEARCHRULENUMERICAL_H
