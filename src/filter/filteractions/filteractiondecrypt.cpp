/*
 * SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
    auto w = new QWidget(parent);
    auto l = new QVBoxLayout(w);

    auto lbl = new QLabel(w);

    QPalette palette = lbl->palette();
    palette.setColor(lbl->foregroundRole(), KColorScheme(QPalette::Normal).foreground(KColorScheme::NegativeText).color());
    lbl->setPalette(palette);
    lbl->setWordWrap(true);

    lbl->setText(i18n("<b>Warning:</b> Decrypted emails may be uploaded to a server!"));
    lbl->setToolTip(
        i18n("<p>If the email folder that you are filtering into is connected to a remote "
             "account (like an IMAP-Server) the decrypted content will go there.</p>"));
    l->addWidget(lbl);

    return w;
}
