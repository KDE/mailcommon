/*
 * SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTION_WITH_CRYPTO_H_
#define MAILCOMMON_FILTERACTION_WITH_CRYPTO_H_

#include "filteraction.h"

#include <gpgme++/global.h>

namespace MailCommon
{
class FilterActionWithCrypto : public FilterAction
{
    Q_OBJECT

protected:
    using FilterAction::FilterAction;

    Q_REQUIRED_RESULT QStringList getEncryptionKeysFromContent(const KMime::Message::Ptr &msg, GpgME::Protocol proto) const;

private:
    // cached values
    mutable QString mGpgSmPath;
    mutable QString mGpgPath;
};
} // namespace MailCommon

#endif
