/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchruledate.h"

#include "filter/filterlog.h"
using MailCommon::FilterLog;

#include <KLocalizedString>
#include <KMime/KMimeMessage>
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
    if (!item.hasPayload<KMime::Message::Ptr>()) {
        return false;
    }
    const auto msg = item.payload<KMime::Message::Ptr>();

    const QDate msgDate = msg->date()->dateTime().date();
    const QDate dateValue = QDate::fromString(contents(), Qt::ISODate);
    bool rc = matchesInternal(dateValue, msgDate);
    if (FilterLog::instance()->isLogging()) {
        QString msg = (rc ? QStringLiteral("<font color=#00FF00>1 = </font>") : QStringLiteral("<font color=#FF0000>0 = </font>"));
        msg += FilterLog::recode(asString());
        msg += QLatin1String(" ( <i>") + contents() + QLatin1String("</i> )"); // TODO change with locale?
        FilterLog::instance()->add(msg, FilterLog::RuleResult);
    }
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
