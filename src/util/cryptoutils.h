/*
 * Copyright (c) 2017  Daniel Vrátil <dvratil@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef MAILCOMMON_CRYPTOUTILS_H_
#define MAILCOMMON_CRYPTOUTILS_H_

#include <KMime/Message>

#include <mailcommon_export.h>

namespace MailCommon {
namespace CryptoUtils {
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
