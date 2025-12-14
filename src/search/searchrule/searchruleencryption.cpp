/*
  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchruleencryption.h"
#include "util/cryptoutils.h"
#include <KMime/Message>

using namespace MailCommon;

SearchRuleEncryption::SearchRuleEncryption(const QByteArray &field, Function func, const QString &contents)
    : SearchRule(field, func, contents)
{
}

SearchRuleEncryption::~SearchRuleEncryption() = default;

bool SearchRuleEncryption::isEmpty() const
{
    // It's true or false, so it can't be empty
    return false;
}

bool SearchRuleEncryption::matches(const Akonadi::Item &item) const
{
    if (!item.hasPayload<QSharedPointer<KMime::Message>>()) {
        return false;
    }
    const auto msg = item.payload<QSharedPointer<KMime::Message>>();

    const bool shouldBeEncrypted = function() == FuncEquals;
    const bool rc = shouldBeEncrypted == CryptoUtils::isEncrypted(msg.data());

    maybeLogMatchResult(rc);
    return rc;
}

SearchRule::RequiredPart SearchRuleEncryption::requiredPart() const
{
    // We can't detect inline signatures just from headers, we need to inspect
    // the entire body.
    return SearchRule::CompleteMessage;
}
