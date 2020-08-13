/*
  SPDX-FileCopyrightText: 2009 Michael Leupold <lemma@confuego.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mdnadvicedialog.h"
#include "kernel/mailkernel.h"
#include "mailcommon_debug.h"

#include <MessageComposer/MessageFactoryNG>
#include <MessageComposer/Util>
using MessageComposer::MessageFactoryNG;

#include <MessageViewer/MessageViewerSettings>
#include <kguiaddons_version.h>
#if KGUIADDONS_VERSION < QT_VERSION_CHECK(5, 73, 0)
#ifndef QT_NO_CURSOR
#include <Libkdepim/KCursorSaver>
#endif
#else
#include <KCursorSaver>
#endif
#include <MimeTreeParser/ObjectTreeParser>

#include <ItemFetchJob>
#include <ItemModifyJob>

#include <KLocalizedString>
#include <KMessageBox>

#include <QPointer>

#include <QDialogButtonBox>
#include <QPushButton>

using namespace MailCommon;

MDNAdviceHelper *MDNAdviceHelper::s_instance = nullptr;

static const struct {
    const char *dontAskAgainID;
    bool canDeny;
    const char *text;
} mdnMessageBoxes[] = {
    {
        "mdnNormalAsk", true,
        I18N_NOOP("This message contains a request to return a notification "
                  "about your reception of the message.\n"
                  "You can either ignore the request or let the mail program "
                  "send a \"denied\" or normal response.")
    },
    {
        "mdnUnknownOption", false,
        I18N_NOOP("This message contains a request to send a notification "
                  "about your reception of the message.\n"
                  "It contains a processing instruction that is marked as "
                  "\"required\", but which is unknown to the mail program.\n"
                  "You can either ignore the request or let the mail program "
                  "send a \"failed\" response.")
    },
    {
        "mdnMultipleAddressesInReceiptTo", true,
        I18N_NOOP("This message contains a request to send a notification "
                  "about your reception of the message,\n"
                  "but it is requested to send the notification to more "
                  "than one address.\n"
                  "You can either ignore the request or let the mail program "
                  "send a \"denied\" or normal response.")
    },
    {
        "mdnReturnPathEmpty", true,
        I18N_NOOP("This message contains a request to send a notification "
                  "about your reception of the message,\n"
                  "but there is no return-path set.\n"
                  "You can either ignore the request or let the mail program "
                  "send a \"denied\" or normal response.")
    },
    {
        "mdnReturnPathNotInReceiptTo", true,
        I18N_NOOP("This message contains a request to send a notification "
                  "about your reception of the message,\n"
                  "but the return-path address differs from the address "
                  "the notification was requested to be sent to.\n"
                  "You can either ignore the request or let the mail program "
                  "send a \"denied\" or normal response.")
    },
};

static const int numMdnMessageBoxes
    = sizeof mdnMessageBoxes / sizeof *mdnMessageBoxes;

MDNAdviceDialog::MDNAdviceDialog(const QString &text, bool canDeny, QWidget *parent)
    : QDialog(parent)
    , m_result(MessageComposer::MDNIgnore)
{
    setWindowTitle(i18nc("@title:window", "Message Disposition Notification Request"));
    QPushButton *user1Button = nullptr;
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Yes, this);
    user1Button = new QPushButton;
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(user1Button, &QPushButton::clicked, this, &MDNAdviceDialog::slotUser1Clicked);
    if (canDeny) {
        QPushButton *user2Button = new QPushButton;
        connect(user2Button, &QPushButton::clicked, this, &MDNAdviceDialog::slotUser2Clicked);
        buttonBox->addButton(user2Button, QDialogButtonBox::ActionRole);
        user2Button->setText(i18n("Send \"&denied\""));
    }
    buttonBox->button(QDialogButtonBox::Yes)->setText(i18n("&Ignore"));
    connect(buttonBox->button(QDialogButtonBox::Yes), &QPushButton::clicked, this, &MDNAdviceDialog::slotYesClicked);
    user1Button->setText(i18n("&Send"));
    buttonBox->button(QDialogButtonBox::Yes)->setShortcut(Qt::Key_Escape);
    KMessageBox::createKMessageBox(
        this, buttonBox,
        QMessageBox::Question,
        text,
        QStringList(),
        QString(),
        nullptr,
        KMessageBox::NoExec);
}

MDNAdviceDialog::~MDNAdviceDialog()
{
}

MessageComposer::MDNAdvice MDNAdviceDialog::result() const
{
    return m_result;
}

MessageComposer::MDNAdvice MDNAdviceHelper::questionIgnoreSend(const QString &text, bool canDeny)
{
    MessageComposer::MDNAdvice rc = MessageComposer::MDNIgnore;
    QPointer<MDNAdviceDialog> dlg(new MDNAdviceDialog(text, canDeny));
    dlg->exec();
    if (dlg) {
        rc = dlg->result();
    }
    delete dlg;
    return rc;
}

QPair< bool, KMime::MDN::SendingMode > MDNAdviceHelper::checkAndSetMDNInfo(
    const Akonadi::Item &item, KMime::MDN::DispositionType d, bool forceSend)
{
    KMime::Message::Ptr msg = MessageComposer::Util::message(item);

    // RFC 2298: At most one MDN may be issued on behalf of each
    // particular recipient by their user agent.  That is, once an MDN
    // has been issued on behalf of a recipient, no further MDNs may be
    // issued on behalf of that recipient, even if another disposition
    // is performed on the message.
    if (item.hasAttribute< MailCommon::MDNStateAttribute >()
        && item.attribute< MailCommon::MDNStateAttribute >()->mdnState() != MailCommon::MDNStateAttribute::MDNStateUnknown) {
        // if already dealt with, don't do it again.
        return QPair< bool, KMime::MDN::SendingMode >(false, KMime::MDN::SentAutomatically);
    }
    MailCommon::MDNStateAttribute *mdnStateAttr
        = new MailCommon::MDNStateAttribute(MailCommon::MDNStateAttribute::MDNStateUnknown);

    KMime::MDN::SendingMode s = KMime::MDN::SentAutomatically; // set to manual if asked user
    bool doSend = false;
    // default:
    int mode = MessageViewer::MessageViewerSettings::self()->defaultPolicy();
    if (forceSend) {   //We must send it
        mode = 3;
    } else {
        if (!mode || mode < 0 || mode > 3) {
            // early out for ignore:
            mdnStateAttr->setMDNState(MailCommon::MDNStateAttribute::MDNIgnore);
            s = KMime::MDN::SentManually;
        } else {
            if (MessageFactoryNG::MDNMDNUnknownOption(msg)) {
                mode = requestAdviceOnMDN("mdnUnknownOption");
                s = KMime::MDN::SentManually;
                // TODO set type to Failed as well
                //      and clear modifiers
            }

            if (MessageFactoryNG::MDNConfirmMultipleRecipients(msg)) {
                mode = requestAdviceOnMDN("mdnMultipleAddressesInReceiptTo");
                s = KMime::MDN::SentManually;
            }

            if (MessageFactoryNG::MDNReturnPathEmpty(msg)) {
                mode = requestAdviceOnMDN("mdnReturnPathEmpty");
                s = KMime::MDN::SentManually;
            }

            if (MessageFactoryNG::MDNReturnPathNotInRecieptTo(msg)) {
                mode = requestAdviceOnMDN("mdnReturnPathNotInReceiptTo");
                s = KMime::MDN::SentManually;
            }

            if (MessageFactoryNG::MDNRequested(msg)) {
                if (s != KMime::MDN::SentManually) {
                    // don't ask again if user has already been asked. use the users' decision
                    mode = requestAdviceOnMDN("mdnNormalAsk");
                    s = KMime::MDN::SentManually; // asked user
                }
            } else { // if message doesn't have a disposition header, never send anything.
                mode = 0;
            }
        }
    }

    // RFC 2298: An MDN MUST NOT be generated in response to an MDN.
    if (MessageComposer::Util::findTypeInMessage(msg.data(),
                                                 "message", "disposition-notification")) {
        mdnStateAttr->setMDNState(MailCommon::MDNStateAttribute::MDNIgnore);
    } else if (mode == 0) {   // ignore
        doSend = false;
        mdnStateAttr->setMDNState(MailCommon::MDNStateAttribute::MDNIgnore);
    } else if (mode == 2) {   // denied
        doSend = true;
        mdnStateAttr->setMDNState(MailCommon::MDNStateAttribute::MDNDenied);
    } else if (mode == 3) {   // the user wants to send. let's make sure we can, according to the RFC.
        doSend = true;
        mdnStateAttr->setMDNState(dispositionToSentState(d));
    }

    // create a minimal version of item with just the attribute we want to change
    Akonadi::Item i(item.id());
    i.setRevision(item.revision());
    i.setMimeType(item.mimeType());
    i.addAttribute(mdnStateAttr);
    Akonadi::ItemModifyJob *modify = new Akonadi::ItemModifyJob(i);
    modify->setIgnorePayload(true);
    modify->disableRevisionCheck();
    return QPair< bool, KMime::MDN::SendingMode >(doSend, s);
}

MailCommon::MDNStateAttribute::MDNSentState MDNAdviceHelper::dispositionToSentState(
    KMime::MDN::DispositionType d)
{
    switch (d) {
    case KMime::MDN::Displayed:
        return MailCommon::MDNStateAttribute::MDNDisplayed;
    case KMime::MDN::Deleted:
        return MailCommon::MDNStateAttribute::MDNDeleted;
    case KMime::MDN::Dispatched:
        return MailCommon::MDNStateAttribute::MDNDispatched;
    case KMime::MDN::Processed:
        return MailCommon::MDNStateAttribute::MDNProcessed;
    case KMime::MDN::Denied:
        return MailCommon::MDNStateAttribute::MDNDenied;
    case KMime::MDN::Failed:
        return MailCommon::MDNStateAttribute::MDNFailed;
    default:
        return MailCommon::MDNStateAttribute::MDNStateUnknown;
    }
}

int MDNAdviceHelper::requestAdviceOnMDN(const char *what)
{
    for (int i = 0; i < numMdnMessageBoxes; ++i) {
        if (!qstrcmp(what, mdnMessageBoxes[i].dontAskAgainID)) {
#if KGUIADDONS_VERSION < QT_VERSION_CHECK(5, 73, 0)
#ifndef QT_NO_CURSOR
            const KPIM::KCursorSaver saver(Qt::ArrowCursor);
#endif
#else
            KCursorSaver saver(Qt::ArrowCursor);
#endif
            MessageComposer::MDNAdvice answer;
            answer = questionIgnoreSend(i18n(mdnMessageBoxes[i].text),
                                        mdnMessageBoxes[i].canDeny);
            switch (answer) {
            case MessageComposer::MDNSend:
                return 3;

            case MessageComposer::MDNSendDenied:
                return 2;

            // don't use 1, as that's used for 'default ask" in checkMDNHeaders
            default:
            case MessageComposer::MDNIgnore:
                return 0;
            }
        }
    }
    qCWarning(MAILCOMMON_LOG) << "didn't find data for message box \""  << what << "\"";
    return MessageComposer::MDNIgnore;
}

void MDNAdviceDialog::slotUser1Clicked()
{
    m_result = MessageComposer::MDNSend;
    accept();
}

void MDNAdviceDialog::slotUser2Clicked()
{
    m_result = MessageComposer::MDNSendDenied;
    accept();
}

void MDNAdviceDialog::slotYesClicked()
{
    m_result = MessageComposer::MDNIgnore;
    accept();
}
