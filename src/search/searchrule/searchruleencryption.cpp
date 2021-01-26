/*
  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchruleencryption.h"
#include "filter/filterlog.h"
#include "util/cryptoutils.h"
using MailCommon::FilterLog;
#include <KMime/Message>

using namespace MailCommon;

SearchRuleEncryption::SearchRuleEncryption(const QByteArray &field, Function func, const QString &contents)
    : SearchRule(field, func, contents)
{
}

SearchRuleEncryption::~SearchRuleEncryption()
{
}

bool SearchRuleEncryption::isEmpty() const
{
    // It's true or false, so it can't be empty
    return false;
}

bool SearchRuleEncryption::matches(const Akonadi::Item &item) const
{
    const bool shouldBeEncrypted = (function() == FuncEquals);

    if (!item.hasPayload<KMime::Message::Ptr>()) {
        return false;
    }
    const auto msg = item.payload<KMime::Message::Ptr>();

    const bool rc = (shouldBeEncrypted == CryptoUtils::isEncrypted(msg.data()));
    if (FilterLog::instance()->isLogging()) {
        QString msg = (rc ? QStringLiteral("<font color=#00FF00>1 = </font>") : QStringLiteral("<font color=#FF0000>0 = </font>"));
        msg += FilterLog::recode(asString());
        msg += QLatin1String(" ( <i>") + contents() + QLatin1String("</i> )"); // TODO change with locale?
        FilterLog::instance()->add(msg, FilterLog::RuleResult);
    }
    return rc;
}

SearchRule::RequiredPart SearchRuleEncryption::requiredPart() const
{
    // We can't detect inline signatures just from headers, we need to inspect
    // the entire body.
    return SearchRule::CompleteMessage;
}
