/*
 * SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteraction.h"

#include <gpgme++/global.h>

namespace MailCommon
{
class FilterActionWithCrypto : public FilterAction
{
    Q_OBJECT

protected:
    using FilterAction::FilterAction;

    [[nodiscard]] QStringList getEncryptionKeysFromContent(const KMime::Message::Ptr &msg, GpgME::Protocol proto) const;

private:
    // cached values
    mutable QString mGpgSmPath;
    mutable QString mGpgPath;
};
} // namespace MailCommon
