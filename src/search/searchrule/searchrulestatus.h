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
/**
 *  This class represents a search to be performed against the status of a
 *  message. The status is represented by a bitfield.
 *
 *  @short This class represents a search pattern rule operating on message
 *  status.
 */
class MAILCOMMON_EXPORT SearchRuleStatus : public SearchRule
{
public:
    explicit SearchRuleStatus(const QByteArray &field = QByteArray(), Function function = FuncContains, const QString &contents = QString());

    explicit SearchRuleStatus(Akonadi::MessageStatus status, Function function = FuncContains);

    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] bool matches(const Akonadi::Item &item) const override;

    /**
     * @copydoc SearchRule::requiredPart()
     */
    [[nodiscard]] RequiredPart requiredPart() const override;

    void addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const override;

    // Not possible to implement optimized form for status searching
    using SearchRule::matches;

    static Akonadi::MessageStatus statusFromEnglishName(const QString &);

    [[nodiscard]] QString informationAboutNotValidRules() const override;

private:
    Akonadi::MessageStatus mStatus;
};
}
