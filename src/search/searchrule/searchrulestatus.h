/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon/searchpattern.h"
#include "mailcommon_export.h"
#include <Akonadi/Item>
#include <Akonadi/MessageStatus>

namespace MailCommon
{
/*!
 * \class MailCommon::SearchRuleStatus
 * \inmodule MailCommon
 * \inheaderfile MailCommon/SearchRuleStatus
 *
 *  This class represents a search to be performed against the status of a
 *  message. The status is represented by a bitfield.
 *
 *  \brief This class represents a search pattern rule operating on message
 *  status.
 */
class MAILCOMMON_EXPORT SearchRuleStatus : public SearchRule
{
public:
    /*!
     * Constructs a search rule for message status.
     *
     * \param field The field to search in
     * \param function The function to use for searching
     * \param contents The contents to search for
     */
    explicit SearchRuleStatus(const QByteArray &field = QByteArray(), Function function = FuncContains, const QString &contents = QString());

    /*!
     * Constructs a search rule for a specific message status.
     *
     * \param status The message status to search for
     * \param function The function to use for searching
     */
    explicit SearchRuleStatus(Akonadi::MessageStatus status, Function function = FuncContains);

    /*!
     * Determines whether the rule is worth considering.
     *
     * \return True if the rule is empty, false otherwise
     */
    [[nodiscard]] bool isEmpty() const override;
    /*!
     * Tries to match the rule against the Akonadi item.
     *
     * \param item The Akonadi item to match against
     * \return True if the rule matches, false otherwise
     */
    [[nodiscard]] bool matches(const Akonadi::Item &item) const override;

    /*!
     * Returns the required part from the item that is needed for the search to operate.
     */
    [[nodiscard]] RequiredPart requiredPart() const override;

    /*!
     * Adds query terms to the given term group.
     *
     * \param groupTerm The search term group to add query terms to
     * \param emptyIsNotAnError Whether empty values should be considered an error
     */
    void addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const override;

    // Not possible to implement optimized form for status searching
    /*!
     * Using parent class matches method for optimization.
     */
    using SearchRule::matches;

    /*!
     * Converts an English status name to the corresponding MessageStatus.
     *
     * \param name The English name of the status
     * \return The corresponding message status
     */
    static Akonadi::MessageStatus statusFromEnglishName(const QString &name);

    /*!
     * Returns information about validation errors in this rule.
     *
     * \return A string describing validation errors, or empty if valid
     */
    [[nodiscard]] QString informationAboutNotValidRules() const override;

private:
    Akonadi::MessageStatus mStatus;
};
}
