/*
  Copyright (c) 2017  Daniel Vrátil <dvratil@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

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

    return shouldBeEncrypted == CryptoUtils::isEncrypted(msg.data());
}

SearchRule::RequiredPart SearchRuleEncryption::requiredPart() const
{
    // We can't detect inline signatures jsut from headers, we need to inspect
    // the entire body.
    return SearchRule::CompleteMessage;
}
