/*
  SPDX-FileCopyrightText: 2009 Michael Leupold <lemma@confuego.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_MDNADVICEDIALOG_H
#define MAILCOMMON_MDNADVICEDIALOG_H

#include "mailcommon_export.h"

#include <MessageComposer/MessageFactoryNG>

#include <QDialog>

namespace MailCommon
{
class MAILCOMMON_EXPORT MDNAdviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MDNAdviceDialog(const QString &text, bool canDeny, QWidget *parent = nullptr);
    ~MDNAdviceDialog() override;

    Q_REQUIRED_RESULT MessageComposer::MDNAdvice result() const;

private:
    void slotUser1Clicked();
    void slotUser2Clicked();
    void slotYesClicked();
    MessageComposer::MDNAdvice m_result = MessageComposer::MDNIgnore;
};
}

#endif
