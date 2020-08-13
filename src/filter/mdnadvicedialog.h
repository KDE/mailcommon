/*
  SPDX-FileCopyrightText: 2009 Michael Leupold <lemma@confuego.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_MDNADVICEDIALOG_H
#define MAILCOMMON_MDNADVICEDIALOG_H

#include "mailcommon_export.h"

#include <MessageComposer/MessageFactoryNG>
#include "mailcommon/mdnstateattribute.h"

#include <KMime/KMimeMessage>

#include <QDialog>

namespace MailCommon {
class MDNAdviceHelper : public QObject
{
    Q_OBJECT
public:
    static MDNAdviceHelper *instance()
    {
        if (!s_instance) {
            s_instance = new MDNAdviceHelper;
        }

        return s_instance;
    }

    /**
     * Checks the MDN headers to see if the user needs to be asked for any
     * confirmations. Will ask the user if action is required.
     *
     * Returns whether to send an MDN or not, and the sending mode for the MDN
     * to be created.
     *
     * Will also set the MailCommon::MDNStateAttribute on the given item
     * to what the user has selected.
     */
    Q_REQUIRED_RESULT QPair<bool, KMime::MDN::SendingMode> checkAndSetMDNInfo(
        const Akonadi::Item &item, KMime::MDN::DispositionType d, bool forceSend = false);

    Q_REQUIRED_RESULT MailCommon::MDNStateAttribute::MDNSentState dispositionToSentState(
        KMime::MDN::DispositionType d);

private:
    explicit MDNAdviceHelper(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    virtual ~MDNAdviceHelper()
    {
    }

    int requestAdviceOnMDN(const char *what);
    MessageComposer::MDNAdvice questionIgnoreSend(const QString &text, bool canDeny);

    static MDNAdviceHelper *s_instance;
};

class MAILCOMMON_EXPORT MDNAdviceDialog : public QDialog
{
    Q_OBJECT

public:
    MDNAdviceDialog(const QString &text, bool canDeny, QWidget *parent = nullptr);
    ~MDNAdviceDialog();

    MessageComposer::MDNAdvice result() const;

private:
    void slotUser1Clicked();
    void slotUser2Clicked();
    void slotYesClicked();
    MessageComposer::MDNAdvice m_result;

protected:

    // Reimplemented
    void slotButtonClicked(int button);
};
}

#endif
