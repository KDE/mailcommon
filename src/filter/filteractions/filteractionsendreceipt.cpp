/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionsendreceipt.h"

#include "kernel/mailkernel.h"
#include "util/mailutil.h"

#include <MessageComposer/MessageFactoryNG>
#include <MessageComposer/MessageSender>

#include <KLocalizedString>

using namespace MailCommon;

FilterActionSendReceipt::FilterActionSendReceipt(QObject *parent)
    : FilterActionWithNone(QStringLiteral("confirm delivery"), i18n("Confirm Delivery"), parent)
{
}

FilterAction::ReturnCode FilterActionSendReceipt::process(ItemContext &context, bool) const
{
    const auto msg = context.item().payload<KMime::Message::Ptr>();

    MessageComposer::MessageFactoryNG factory(msg, context.item().id());
    factory.setFolderIdentity(Util::folderIdentity(context.item()));
    factory.setIdentityManager(KernelIf->identityManager());

    const KMime::Message::Ptr receipt = factory.createDeliveryReceipt();
    if (!receipt) {
        return ErrorButGoOn;
    }

    // Queue message. This is a) so that the user can check
    // the receipt before sending and b) for speed reasons.
    KernelIf->msgSender()->send(receipt, MessageComposer::MessageSender::SendLater);

    return GoOn;
}

SearchRule::RequiredPart FilterActionSendReceipt::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

FilterAction *FilterActionSendReceipt::newAction()
{
    return new FilterActionSendReceipt;
}
