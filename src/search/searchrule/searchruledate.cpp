/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchruledate.h"

#include <KLocalizedString>
#include <KMime/Message>
using namespace MailCommon;

SearchRuleDate::SearchRuleDate(const QByteArray &field, Function func, const QString &contents)
    : SearchRule(field, func, contents)
{
}

QString SearchRuleDate::informationAboutNotValidRules() const
{
    return i18n("Date is not valid.");
}

bool SearchRuleDate::isEmpty() const
{
    return !QDate::fromString(contents(), Qt::ISODate).isValid();
}

bool SearchRuleDate::matches(const Akonadi::Item &item) const
{
    if (!item.hasPayload<std::shared_ptr<KMime::Message>>()) {
        return false;
    }
    const auto msg = item.payload<std::shared_ptr<KMime::Message>>();

    const QDate msgDate = msg->date()->dateTime().date();
    const QDate dateValue = QDate::fromString(contents(), Qt::ISODate);
    const bool rc = matchesInternal(dateValue, msgDate);

    maybeLogMatchResult(rc);
    return rc;
}

bool SearchRuleDate::matchesInternal(QDate dateValue, QDate msgDate) const
{
    switch (function()) {
    case SearchRule::FuncEquals:
        return dateValue == msgDate;

    case SearchRule::FuncNotEqual:
        return dateValue != msgDate;

    case FuncIsGreater:
        return msgDate > dateValue;

    case FuncIsLessOrEqual:
        return msgDate <= dateValue;

    case FuncIsLess:
        return msgDate < dateValue;

    case FuncIsGreaterOrEqual:
        return msgDate >= dateValue;

    default:;
    }
    return false;
}

SearchRule::RequiredPart SearchRuleDate::requiredPart() const
{
    return SearchRule::Envelope;
}

void SearchRuleDate::addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const
{
    using namespace Akonadi;
    emptyIsNotAnError = false;

    const QDate date = QDate::fromString(contents(), Qt::ISODate);
    EmailSearchTerm term(EmailSearchTerm::HeaderOnlyDate, date, akonadiComparator());
    term.setIsNegated(isNegated());
    groupTerm.addSubTerm(term);
}
