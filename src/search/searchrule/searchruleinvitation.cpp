/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchruleinvitation.h"
#include "filter/filterlog.h"
using MailCommon::FilterLog;
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
    if (!item.hasPayload<KMime::Message::Ptr>()) {
        return false;
    }
    Akonadi::MessageStatus status;
    status.setStatusFromFlags(item.flags());
    bool rc = false;
    switch (function()) {
    case FuncEquals:
        if (status & Akonadi::MessageStatus::statusHasInvitation()) {
            rc = true;
        }
        break;
    case FuncNotEqual:
        if (!(status & Akonadi::MessageStatus::statusHasInvitation())) {
            rc = true;
        }
        break;
    default:
        break;
    }

    if (FilterLog::instance()->isLogging()) {
        QString msg = (rc ? QStringLiteral("<font color=#00FF00>1 = </font>") : QStringLiteral("<font color=#FF0000>0 = </font>"));
        msg += FilterLog::recode(asString());
        msg += QLatin1StringView(" ( <i>") + contents() + QLatin1StringView("</i> )"); // TODO change with locale?
        FilterLog::instance()->add(msg, FilterLog::RuleResult);
    }
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
