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

#include "filteractiondecrypt.h"
#include "mailcommon_debug.h"
#include "util/cryptoutils.h"

#include <KColorScheme>
#include <KLocalizedString>

#include <KMime/Message>

#include <gpgme++/decryptionresult.h>

#include <Akonadi/KMime/MessageFlags>

#include <QLabel>
#include <QVBoxLayout>

using namespace MailCommon;

FilterActionDecrypt::FilterActionDecrypt(QObject *parent)
    : FilterActionWithCrypto(QStringLiteral("decrypt"), i18n("Decrypt"), parent)
{
}

FilterActionDecrypt::~FilterActionDecrypt()
{
}

FilterAction *FilterActionDecrypt::newAction()
{
    return new FilterActionDecrypt();
}

QString FilterActionDecrypt::displayString() const
{
    return i18n("Decrypt");
}

QString FilterActionDecrypt::argsAsString() const
{
    return {};
}

void FilterActionDecrypt::argsFromString(const QString &)
{
}

SearchRule::RequiredPart FilterActionDecrypt::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

FilterAction::ReturnCode FilterActionDecrypt::process(ItemContext &context, bool) const
{
    auto &item = context.item();
    if (!item.hasPayload<KMime::Message::Ptr>()) {
        return ErrorNeedComplete;
    }

    auto msg = item.payload<KMime::Message::Ptr>();
    if (!isEncrypted(msg.data())) {
        qCDebug(MAILCOMMON_LOG) << "Message not encrypted";
        return GoOn;
    }

    bool wasEncrypted;
    auto nec = CryptoUtils::decryptMessage(msg, wasEncrypted);
    if (!nec) {
        return wasEncrypted ? ErrorButGoOn : GoOn;
    }

    context.item().setPayload(nec);
    context.item().clearFlag(Akonadi::MessageFlags::Encrypted);
    context.setNeedsPayloadStore();
    context.setNeedsFlagStore();
    return GoOn;
}

QWidget *FilterActionDecrypt::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QVBoxLayout *l = new QVBoxLayout(w);

    QLabel *lbl = new QLabel(w);

    QPalette palette = lbl->palette();
    palette.setColor(lbl->foregroundRole(), KColorScheme(QPalette::Normal).foreground(KColorScheme::NegativeText).color());
    lbl->setPalette(palette);
    lbl->setWordWrap(true);

    lbl->setText(i18n("<b>Warning:</b> Decrypted emails may be uploaded to a server!"));
    lbl->setToolTip(i18n("<p>If the email folder that you are filtering into is connected to a remote "
                         "account (like an IMAP-Server) the decrypted content will go there.</p>"));
    l->addWidget(lbl);

    return w;
}
