/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mdnwarningjob.h"
#include "kernel/mailkernel.h"
#include "mailcommon_debug.h"
#include "util/mailutil.h"
#include <Akonadi/ItemModifyJob>
#include <Akonadi/MDNStateAttribute>
#include <MessageComposer/MessageSender>
#include <MessageComposer/Util>
#include <MessageViewer/MessageViewerSettings>
using namespace MailCommon;
MDNWarningJob::MDNWarningJob(IKernel *kernel, QObject *parent)
    : QObject{parent}
    , mKernel(kernel)
{
}

MDNWarningJob::~MDNWarningJob()
{
}

void MDNWarningJob::start()
{
    if (!canStart()) {
        deleteLater();
        return;
    }
    const Akonadi::Collection collection = mItem.parentCollection();
    if (collection.isValid()
        && (CommonKernel->folderIsSentMailFolder(collection) || CommonKernel->folderIsTrash(collection) || CommonKernel->folderIsDraftOrOutbox(collection)
            || CommonKernel->folderIsTemplates(collection))) {
        deleteLater();
        return;
    }

    const KMime::Message::Ptr message = MessageComposer::Util::message(mItem);
    if (!message) {
        deleteLater();
        return;
    }

    // TODO modifyItem();

    const QPair<bool, KMime::MDN::SendingMode> mdnSend = MessageComposer::MDNAdviceHelper::instance()->checkAndSetMDNInfo(mItem, KMime::MDN::Displayed);
    if (mdnSend.first) {
        const int quote = MessageViewer::MessageViewerSettings::self()->quoteMessage();

        MessageComposer::MessageFactoryNG factory(message, Akonadi::Item().id());
        factory.setIdentityManager(mKernel->identityManager());
        factory.setFolderIdentity(MailCommon::Util::folderIdentity(mItem));

        const KMime::Message::Ptr mdn = factory.createMDN(KMime::MDN::ManualAction, KMime::MDN::Displayed, mdnSend.second, quote);
        if (mdn) {
            if (!mKernel->msgSender()->send(mdn)) {
                qCDebug(MAILCOMMON_LOG) << "Sending failed.";
            }
        }
    }
    Q_EMIT finished();
    deleteLater();
}

const Akonadi::Item &MDNWarningJob::item() const
{
    return mItem;
}

void MDNWarningJob::setItem(const Akonadi::Item &newItem)
{
    mItem = newItem;
}

bool MDNWarningJob::canStart() const
{
    return mItem.isValid() && (mResponse != Unknown);
}

QPair<bool, KMime::MDN::SendingMode> MDNWarningJob::modifyItem()
{
    QPair<bool, KMime::MDN::SendingMode> result;
    const KMime::Message::Ptr msg = MessageComposer::Util::message(mItem);
    auto mdnStateAttr = new Akonadi::MDNStateAttribute(Akonadi::MDNStateAttribute::MDNStateUnknown);
    // create a minimal version of item with just the attribute we want to change
#if 0
    bool doSend = false;
    // RFC 2298: An MDN MUST NOT be generated in response to an MDN.
    if (MessageComposer::Util::findTypeInMessage(msg.data(), "message", "disposition-notification")) {
        mdnStateAttr->setMDNState(Akonadi::MDNStateAttribute::MDNIgnore);
    } else if (mode == 0) { // ignore
        doSend = false;
        mdnStateAttr->setMDNState(Akonadi::MDNStateAttribute::MDNIgnore);
    } else if (mode == 2) { // denied
        doSend = true;
        mdnStateAttr->setMDNState(Akonadi::MDNStateAttribute::MDNDenied);
    } else if (mode == 3) { // the user wants to send. let's make sure we can, according to the RFC.
        doSend = true;
        mdnStateAttr->setMDNState(dispositionToSentState(d));
    }
#endif
    Akonadi::Item i(mItem.id());
    i.setRevision(mItem.revision());
    i.setMimeType(mItem.mimeType());
    i.addAttribute(mdnStateAttr);
    auto modify = new Akonadi::ItemModifyJob(i);
    modify->setIgnorePayload(true);
    modify->disableRevisionCheck();
    return result;
}

MDNWarningJob::ResponseMDN MDNWarningJob::response() const
{
    return mResponse;
}

void MDNWarningJob::setResponse(ResponseMDN newResponse)
{
    mResponse = newResponse;
}
