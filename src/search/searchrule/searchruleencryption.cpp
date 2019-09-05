/*
  Copyright (c) 2017  Daniel Vrátil <dvratil@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "searchruleencryption.h"
#include "util/cryptoutils.h"
#include "filter/filterlog.h"
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

    bool rc = (shouldBeEncrypted == CryptoUtils::isEncrypted(msg.data()));
    if (FilterLog::instance()->isLogging()) {
        QString msg = (rc ? QStringLiteral("<font color=#00FF00>1 = </font>")
                       : QStringLiteral("<font color=#FF0000>0 = </font>"));
        msg += FilterLog::recode(asString());
        msg += QLatin1String(" ( <i>") + contents() + QLatin1String("</i> )"); //TODO change with locale?
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
