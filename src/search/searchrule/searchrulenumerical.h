/*
  SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "mailcommon_private_export.h"
#include "searchpattern.h"
#include <Akonadi/Item>
namespace MailCommon
{
/**
 * @short This class represents a search pattern rule operating on numerical values.
 *
 * This class represents a search to be performed against a numerical value,
 * such as the age of the message in days or its size.
 */
class MAILCOMMON_TESTS_EXPORT SearchRuleNumerical : public SearchRule
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
    [[nodiscard]] bool isEmpty() const override;

    /**
     * @copydoc SearchRule::matches()
     */
    [[nodiscard]] bool matches(const Akonadi::Item &item) const override;

    /**
     * @copydoc SearchRule::requiredPart()
     */
    [[nodiscard]] RequiredPart requiredPart() const override;

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
    [[nodiscard]] QString informationAboutNotValidRules() const override;
};
}
