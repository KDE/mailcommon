/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <Akonadi/Item>

#include "mailcommon/searchpattern.h"
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

    Q_REQUIRED_RESULT bool isEmpty() const override;
    Q_REQUIRED_RESULT bool matches(const Akonadi::Item &item) const override;

    /**
     * @copydoc SearchRule::requiredPart()
     */
    Q_REQUIRED_RESULT RequiredPart requiredPart() const override;

    void addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const override;

    // Not possible to implement optimized form for status searching
    using SearchRule::matches;

    static Akonadi::MessageStatus statusFromEnglishName(const QString &);

    Q_REQUIRED_RESULT QString informationAboutNotValidRules() const override;

private:
    Akonadi::MessageStatus mStatus;
};
}
