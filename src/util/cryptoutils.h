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
[[nodiscard]] MAILCOMMON_EXPORT std::shared_ptr<KMime::Message> assembleMessage(const std::shared_ptr<KMime::Message> &orig, const KMime::Content *newContent);
[[nodiscard]] MAILCOMMON_EXPORT std::shared_ptr<KMime::Message> decryptMessage(const std::shared_ptr<KMime::Message> &decrypt, bool &wasEncrypted);

[[nodiscard]] MAILCOMMON_EXPORT bool isInlinePGP(const KMime::Content *content);
[[nodiscard]] MAILCOMMON_EXPORT bool isPGP(const KMime::Content *content, bool allowOctetStream = false);
[[nodiscard]] MAILCOMMON_EXPORT bool isSMIME(const KMime::Content *content);

[[nodiscard]] MAILCOMMON_EXPORT bool isEncrypted(const KMime::Message *content);

MAILCOMMON_EXPORT void copyHeader(const KMime::Headers::Base *header, std::shared_ptr<KMime::Message> destMsg);
[[nodiscard]] MAILCOMMON_EXPORT bool isContentHeader(const KMime::Headers::Base *header);
}
}
