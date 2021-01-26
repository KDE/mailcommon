/*
 * SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_CRYPTOUTILS_H_
#define MAILCOMMON_CRYPTOUTILS_H_

#include <KMime/Message>

#include <mailcommon_export.h>

namespace MailCommon
{
namespace CryptoUtils
{
Q_REQUIRED_RESULT MAILCOMMON_EXPORT KMime::Message::Ptr assembleMessage(const KMime::Message::Ptr &orig, const KMime::Content *newContent);
Q_REQUIRED_RESULT MAILCOMMON_EXPORT KMime::Message::Ptr decryptMessage(const KMime::Message::Ptr &decrypt, bool &wasEncrypted);

Q_REQUIRED_RESULT MAILCOMMON_EXPORT bool isInlinePGP(const KMime::Content *content);
Q_REQUIRED_RESULT MAILCOMMON_EXPORT bool isPGP(const KMime::Content *content, bool allowOctetStream = false);
Q_REQUIRED_RESULT MAILCOMMON_EXPORT bool isSMIME(const KMime::Content *content);

Q_REQUIRED_RESULT MAILCOMMON_EXPORT bool isEncrypted(const KMime::Message *content);

MAILCOMMON_EXPORT void copyHeader(const KMime::Headers::Base *header, KMime::Message::Ptr destMsg);
Q_REQUIRED_RESULT MAILCOMMON_EXPORT bool isContentHeader(const KMime::Headers::Base *header);
}
}

#endif
