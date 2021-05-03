/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionreplyto.h"

#include <KLocalizedString>

using namespace MailCommon;

FilterAction *FilterActionReplyTo::newAction()
{
    return new FilterActionReplyTo;
}

FilterActionReplyTo::FilterActionReplyTo(QObject *parent)
    : FilterActionWithAddress(QStringLiteral("set Reply-To"), i18n("Set Reply-To To"), parent)
{
}

FilterAction::ReturnCode FilterActionReplyTo::process(ItemContext &context, bool) const
{
    if (mParameter.isEmpty()) {
        return ErrorButGoOn;
    }
    const auto msg = context.item().payload<KMime::Message::Ptr>();
    const QByteArray replyTo("Reply-To");
    KMime::Headers::Base *header = KMime::Headers::createHeader(replyTo);
    if (!header) {
        header = new KMime::Headers::Generic(replyTo.constData());
    }
    header->fromUnicodeString(mParameter, "utf-8");
    msg->setHeader(header);
    msg->assemble();

    context.setNeedsPayloadStore();

    return GoOn;
}

SearchRule::RequiredPart FilterActionReplyTo::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

QString FilterActionReplyTo::informationAboutNotValidAction() const
{
    return i18n("Email address was not defined.");
}
