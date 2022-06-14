/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mdnwarningjob.h"
#include "kernel/mailkernel.h"
#include "mailcommon_debug.h"
#include "util/mailutil.h"
#include <Akonadi/ItemModifyJob>
#include <MessageComposer/MDNAdviceHelper>
#include <MessageComposer/MDNStateAttribute>
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
    return mItem.isValid();
}

void MDNWarningJob::modifyItem()
{
    auto mdnStateAttr = new MessageComposer::MDNStateAttribute(MessageComposer::MDNStateAttribute::MDNStateUnknown);
    // create a minimal version of item with just the attribute we want to change
    Akonadi::Item i(mItem.id());
    i.setRevision(mItem.revision());
    i.setMimeType(mItem.mimeType());
    i.addAttribute(mdnStateAttr);
    auto modify = new Akonadi::ItemModifyJob(i);
    modify->setIgnorePayload(true);
    modify->disableRevisionCheck();
}
