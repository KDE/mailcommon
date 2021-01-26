/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchrulestatus.h"
#include "filter/filterlog.h"
using MailCommon::FilterLog;

#include <KMime/KMimeMessage>

using namespace MailCommon;

struct _statusNames {
    const char *name;
    Akonadi::MessageStatus status;
};

static struct _statusNames statusNames[] = {{"Important", Akonadi::MessageStatus::statusImportant()},
                                            {"Unread", Akonadi::MessageStatus::statusUnread()},
                                            {"Read", Akonadi::MessageStatus::statusRead()},
                                            {"Deleted", Akonadi::MessageStatus::statusDeleted()},
                                            {"Replied", Akonadi::MessageStatus::statusReplied()},
                                            {"Forwarded", Akonadi::MessageStatus::statusForwarded()},
                                            {"Queued", Akonadi::MessageStatus::statusQueued()},
                                            {"Sent", Akonadi::MessageStatus::statusSent()},
                                            {"Watched", Akonadi::MessageStatus::statusWatched()},
                                            {"Ignored", Akonadi::MessageStatus::statusIgnored()},
                                            {"Action Item", Akonadi::MessageStatus::statusToAct()},
                                            {"Spam", Akonadi::MessageStatus::statusSpam()},
                                            {"Ham", Akonadi::MessageStatus::statusHam()},
                                            {"Has Attachment", Akonadi::MessageStatus::statusHasAttachment()}};

QString englishNameForStatus(Akonadi::MessageStatus status)
{
    for (const _statusNames &statusName : statusNames) {
        if (statusName.status == status) {
            return QString::fromLatin1(statusName.name);
        }
    }
    return QString();
}

SearchRuleStatus::SearchRuleStatus(const QByteArray &field, Function func, const QString &aContents)
    : SearchRule(field, func, aContents)
{
    // the values are always in english, both from the conf file as well as
    // the patternedit gui
    mStatus = statusFromEnglishName(aContents);
}

SearchRuleStatus::SearchRuleStatus(Akonadi::MessageStatus status, Function func)
    : SearchRule("<status>", func, englishNameForStatus(status))
{
    mStatus = status;
}

Akonadi::MessageStatus SearchRuleStatus::statusFromEnglishName(const QString &aStatusString)
{
    for (const _statusNames &statusName : statusNames) {
        if (!aStatusString.compare(QString::fromLatin1(statusName.name))) {
            return statusName.status;
        }
    }
    Akonadi::MessageStatus unknown;
    return unknown;
}

QString SearchRuleStatus::informationAboutNotValidRules() const
{
    // TODO
    return QString();
}

bool SearchRuleStatus::isEmpty() const
{
    return field().trimmed().isEmpty() || contents().isEmpty();
}

bool SearchRuleStatus::matches(const Akonadi::Item &item) const
{
    Akonadi::MessageStatus status;
    status.setStatusFromFlags(item.flags());
    bool rc = false;
    switch (function()) {
    case FuncEquals: // fallthrough. So that "<status> 'is' 'read'" works
    case FuncContains:
        if (status & mStatus) {
            rc = true;
        }
        break;
    case FuncNotEqual: // fallthrough. So that "<status> 'is not' 'read'" works
    case FuncContainsNot:
        if (!(status & mStatus)) {
            rc = true;
        }
        break;
    // FIXME what about the remaining funcs, how can they make sense for
    // stati?
    default:
        break;
    }
    if (FilterLog::instance()->isLogging()) {
        QString msg = (rc ? QStringLiteral("<font color=#00FF00>1 = </font>") : QStringLiteral("<font color=#FF0000>0 = </font>"));
        msg += FilterLog::recode(asString());
        FilterLog::instance()->add(msg, FilterLog::RuleResult);
    }
    return rc;
}

SearchRule::RequiredPart SearchRuleStatus::requiredPart() const
{
    return SearchRule::Envelope;
}

void SearchRuleStatus::addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const
{
    using namespace Akonadi;
    emptyIsNotAnError = true;
    // TODO double check that isRead also works
    if (!mStatus.statusFlags().isEmpty()) {
        EmailSearchTerm term(EmailSearchTerm::MessageStatus, mStatus.statusFlags().values().first(), akonadiComparator());
        term.setIsNegated(isNegated());
        groupTerm.addSubTerm(term);
    } else {
        // Special case Unread
        Akonadi::MessageStatus status;
        status.setRead(true);
        EmailSearchTerm term(EmailSearchTerm::MessageStatus, status.statusFlags().values().first(), akonadiComparator());
        term.setIsNegated(!isNegated());
        groupTerm.addSubTerm(term);
    }
}
