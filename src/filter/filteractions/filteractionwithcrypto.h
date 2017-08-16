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

#ifndef MAILCOMMON_FILTERACTION_WITH_CRYPTO_H_
#define MAILCOMMON_FILTERACTION_WITH_CRYPTO_H_

#include "filteraction.h"

#include <gpgme++/global.h>

namespace MailCommon {

class FilterActionWithCrypto : public FilterAction
{
    Q_OBJECT

protected:
    using FilterAction::FilterAction;

    KMime::Message::Ptr assembleMessage(const KMime::Message::Ptr &orig,
                                        const KMime::Content *newContent) const;
    KMime::Message::Ptr decryptMessage(const KMime::Message::Ptr &decrypt,
                                       bool &wasEncrypted) const;

    bool isInlinePGP(const KMime::Content *content) const;
    bool isPGP(const KMime::Content *content, bool allowOctetStream = false) const;
    bool isSMIME(const KMime::Content *content) const;

    QStringList getEncryptionKeysFromContent(const KMime::Message::Ptr &msg, GpgME::Protocol proto) const;

    bool isEncrypted(const KMime::Message *content) const;

private:
    void copyHeader(const KMime::Headers::Base *header,
                    KMime::Message::Ptr destMsg) const;
    bool isContentHeader(const KMime::Headers::Base *header) const;

private:
    // cached values
    mutable QString mGpgSmPath;
    mutable QString mGpgPath;
};

} // namespace MailCommon

#endif
