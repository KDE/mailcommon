/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchruleattachment.h"
#include <Akonadi/MessageStatus>
#include <KMime/Message>

using namespace MailCommon;

SearchRuleAttachment::SearchRuleAttachment(const QByteArray &field, Function func, const QString &contents)
    : SearchRule(field, func, contents)
{
}

SearchRuleAttachment::~SearchRuleAttachment() = default;

bool SearchRuleAttachment::isEmpty() const
{
    // It's true or false, so it can't be empty
    return false;
}

bool SearchRuleAttachment::matches(const Akonadi::Item &item) const
{
    if (!item.hasPayload<std::shared_ptr<KMime::Message>>()) {
        return false;
    }
    Akonadi::MessageStatus status;
    status.setStatusFromFlags(item.flags());
    bool rc = false;
    switch (function()) {
    case FuncEquals:
        rc = status & Akonadi::MessageStatus::statusHasAttachment();
        break;
    case FuncNotEqual:
        rc = !(status & Akonadi::MessageStatus::statusHasAttachment());
        break;
    default:
        break;
    }

    maybeLogMatchResult(rc);
    return rc;
}

SearchRule::RequiredPart SearchRuleAttachment::requiredPart() const
{
    return SearchRule::Envelope;
}

void SearchRuleAttachment::addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const
{
    using namespace Akonadi;
    emptyIsNotAnError = true;
    EmailSearchTerm term(EmailSearchTerm::MessageStatus, MessageStatus::statusHasAttachment().statusFlags().values().first(), akonadiComparator());
    term.setIsNegated(isNegated());
    groupTerm.addSubTerm(term);
}
