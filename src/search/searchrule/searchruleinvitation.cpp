/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchruleinvitation.h"
#include <Akonadi/MessageStatus>
#include <KMime/Message>

using namespace MailCommon;

SearchRuleInvitation::SearchRuleInvitation(const QByteArray &field, Function func, const QString &contents)
    : SearchRule(field, func, contents)
{
}

SearchRuleInvitation::~SearchRuleInvitation() = default;

bool SearchRuleInvitation::isEmpty() const
{
    // It's true or false, so it can't be empty
    return false;
}

bool SearchRuleInvitation::matches(const Akonadi::Item &item) const
{
    if (!item.hasPayload<QSharedPointer<KMime::Message>>()) {
        return false;
    }
    Akonadi::MessageStatus status;
    status.setStatusFromFlags(item.flags());
    bool rc = false;
    switch (function()) {
    case FuncEquals:
        rc = status & Akonadi::MessageStatus::statusHasInvitation();
        break;
    case FuncNotEqual:
        rc = !(status & Akonadi::MessageStatus::statusHasInvitation());
        break;
    default:
        break;
    }

    maybeLogMatchResult(rc);
    return rc;
}

SearchRule::RequiredPart SearchRuleInvitation::requiredPart() const
{
    return SearchRule::Envelope;
}

void SearchRuleInvitation::addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const
{
    using namespace Akonadi;
    emptyIsNotAnError = true;
    EmailSearchTerm term(EmailSearchTerm::MessageStatus, MessageStatus::statusHasInvitation().statusFlags().values().first(), akonadiComparator());
    term.setIsNegated(isNegated());
    groupTerm.addSubTerm(term);
}
