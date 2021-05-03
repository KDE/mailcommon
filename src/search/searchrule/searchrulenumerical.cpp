/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchrulenumerical.h"

#include "filter/filterlog.h"
using MailCommon::FilterLog;

#include <KMime/KMimeMessage>
#include <QDateTime>

#include <QRegularExpression>

#include <algorithm>

using namespace MailCommon;

SearchRuleNumerical::SearchRuleNumerical(const QByteArray &field, Function func, const QString &contents)
    : SearchRule(field, func, contents)
{
}

bool SearchRuleNumerical::isEmpty() const
{
    bool ok = false;
    contents().toLongLong(&ok);

    return !ok;
}

bool SearchRuleNumerical::matches(const Akonadi::Item &item) const
{
    if (!item.hasPayload<KMime::Message::Ptr>()) {
        return false;
    }

    const auto msg = item.payload<KMime::Message::Ptr>();

    QString msgContents;
    qint64 numericalMsgContents = 0;
    qint64 numericalValue = 0;

    if (qstricmp(field().constData(), "<size>") == 0) {
        numericalMsgContents = item.size();
        numericalValue = contents().toLongLong();
        msgContents.setNum(numericalMsgContents);
    } else if (qstricmp(field().constData(), "<age in days>") == 0) {
        QDateTime msgDateTime = msg->date()->dateTime();
        numericalMsgContents = msgDateTime.daysTo(QDateTime::currentDateTime());
        numericalValue = contents().toInt();
        msgContents.setNum(numericalMsgContents);
    } else {
        return false;
    }
    bool rc = matchesInternal(numericalValue, numericalMsgContents, msgContents);
    if (FilterLog::instance()->isLogging()) {
        QString msg = (rc ? QStringLiteral("<font color=#00FF00>1 = </font>") : QStringLiteral("<font color=#FF0000>0 = </font>"));
        msg += FilterLog::recode(asString());
        msg += QLatin1String(" ( <i>") + QString::number(numericalMsgContents) + QLatin1String("</i> )");
        FilterLog::instance()->add(msg, FilterLog::RuleResult);
    }
    return rc;
}

SearchRule::RequiredPart SearchRuleNumerical::requiredPart() const
{
    return SearchRule::Envelope;
}

bool SearchRuleNumerical::matchesInternal(long numericalValue, long numericalMsgContents, const QString &msgContents) const
{
    switch (function()) {
    case SearchRule::FuncEquals:
        return numericalValue == numericalMsgContents;

    case SearchRule::FuncNotEqual:
        return numericalValue != numericalMsgContents;

    case SearchRule::FuncContains:
        return msgContents.contains(contents(), Qt::CaseInsensitive);

    case SearchRule::FuncContainsNot:
        return !msgContents.contains(contents(), Qt::CaseInsensitive);

    case SearchRule::FuncRegExp:
        return msgContents.contains(QRegularExpression(contents(), QRegularExpression::CaseInsensitiveOption));

    case SearchRule::FuncNotRegExp:
        return !msgContents.contains(QRegularExpression(contents(), QRegularExpression::CaseInsensitiveOption));

    case FuncIsGreater:
        return numericalMsgContents > numericalValue;

    case FuncIsLessOrEqual:
        return numericalMsgContents <= numericalValue;

    case FuncIsLess:
        return numericalMsgContents < numericalValue;

    case FuncIsGreaterOrEqual:
        return numericalMsgContents >= numericalValue;

    case FuncIsInAddressbook: // since email-addresses are not numerical, I settle for false here
        return false;

    case FuncIsNotInAddressbook:
        return false;

    default:;
    }

    return false;
}

void SearchRuleNumerical::addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const
{
    using namespace Akonadi;
    emptyIsNotAnError = false;
    if (qstricmp(field().constData(), "<size>") == 0) {
        EmailSearchTerm term(EmailSearchTerm::ByteSize, contents().toInt(), akonadiComparator());
        term.setIsNegated(isNegated());
        groupTerm.addSubTerm(term);
    } else if (qstricmp(field().constData(), "<age in days>") == 0) {
        QDate date(QDate::currentDate());
        date = date.addDays(contents().toInt());
        EmailSearchTerm term(EmailSearchTerm::HeaderOnlyDate, date, akonadiComparator());
        term.setIsNegated(isNegated());
        groupTerm.addSubTerm(term);
    }
}

QString SearchRuleNumerical::informationAboutNotValidRules() const
{
    return i18n("Content is not a number.");
}
