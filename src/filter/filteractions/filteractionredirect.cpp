/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionredirect.h"
#include "kernel/mailkernel.h"
#include "mailcommon_debug.h"
#include "util/mailutil.h"

#include <MessageComposer/MessageFactoryNG>
#include <MessageComposer/MessageSender>
#include <MessageComposer/Util>

#include <KLocalizedString>

using namespace MailCommon;

FilterAction *FilterActionRedirect::newAction()
{
    return new FilterActionRedirect;
}

FilterActionRedirect::FilterActionRedirect(QObject *parent)
    : FilterActionWithAddress(QStringLiteral("redirect"), i18n("Redirect To"), parent)
{
}

FilterAction::ReturnCode FilterActionRedirect::process(ItemContext &context, bool) const
{
    if (isEmpty()) {
        return ErrorButGoOn;
    }

    KMime::Message::Ptr msg = MessageComposer::Util::message(context.item());

    MessageComposer::MessageFactoryNG factory(msg, context.item().id());
    factory.setFolderIdentity(Util::folderIdentity(context.item()));
    factory.setIdentityManager(KernelIf->identityManager());

    KMime::Message::Ptr rmsg = factory.createRedirect(mParameter);
    if (!rmsg) {
        return ErrorButGoOn;
    }

    sendMDN(context.item(), KMime::MDN::Dispatched);

    if (!KernelIf->msgSender()->send(rmsg, MessageComposer::MessageSender::SendLater)) {
        qCDebug(MAILCOMMON_LOG) << "FilterAction: could not redirect message (sending failed)";
        return ErrorButGoOn; // error: couldn't send
    }

    return GoOn;
}

SearchRule::RequiredPart FilterActionRedirect::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

QString FilterActionRedirect::sieveCode() const
{
    const QString result = QStringLiteral("redirect \"%1\";").arg(mParameter);
    return result;
}

QString FilterActionRedirect::informationAboutNotValidAction() const
{
    return i18n("Email address was missing.");
}
