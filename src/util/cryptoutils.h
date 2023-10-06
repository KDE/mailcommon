/*
 * SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include <KMime/Message>

#include "mailcommon_export.h"

namespace MailCommon
{
namespace CryptoUtils
{
[[nodiscard]] MAILCOMMON_EXPORT KMime::Message::Ptr assembleMessage(const KMime::Message::Ptr &orig, const KMime::Content *newContent);
[[nodiscard]] MAILCOMMON_EXPORT KMime::Message::Ptr decryptMessage(const KMime::Message::Ptr &decrypt, bool &wasEncrypted);

[[nodiscard]] MAILCOMMON_EXPORT bool isInlinePGP(const KMime::Content *content);
[[nodiscard]] MAILCOMMON_EXPORT bool isPGP(const KMime::Content *content, bool allowOctetStream = false);
[[nodiscard]] MAILCOMMON_EXPORT bool isSMIME(const KMime::Content *content);

[[nodiscard]] MAILCOMMON_EXPORT bool isEncrypted(const KMime::Message *content);

MAILCOMMON_EXPORT void copyHeader(const KMime::Headers::Base *header, KMime::Message::Ptr destMsg);
[[nodiscard]] MAILCOMMON_EXPORT bool isContentHeader(const KMime::Headers::Base *header);
}
}
