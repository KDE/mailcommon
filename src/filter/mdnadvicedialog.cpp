/*
  SPDX-FileCopyrightText: 2009 Michael Leupold <lemma@confuego.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mdnadvicedialog.h"
#include "mailcommon_debug.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QDialogButtonBox>
#include <QPushButton>

using namespace MailCommon;

MDNAdviceDialog::MDNAdviceDialog(const QString &text, bool canDeny, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Message Disposition Notification Request"));
    QPushButton *user1Button = nullptr;
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Yes, this);
    user1Button = new QPushButton;
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(user1Button, &QPushButton::clicked, this, &MDNAdviceDialog::slotUser1Clicked);
    if (canDeny) {
        auto user2Button = new QPushButton;
        connect(user2Button, &QPushButton::clicked, this, &MDNAdviceDialog::slotUser2Clicked);
        buttonBox->addButton(user2Button, QDialogButtonBox::ActionRole);
        user2Button->setText(i18n("Send \"&denied\""));
    }
    buttonBox->button(QDialogButtonBox::Yes)->setText(i18n("&Ignore"));
    connect(buttonBox->button(QDialogButtonBox::Yes), &QPushButton::clicked, this, &MDNAdviceDialog::slotYesClicked);
    user1Button->setText(i18n("&Send"));
    buttonBox->button(QDialogButtonBox::Yes)->setShortcut(Qt::Key_Escape);
    KMessageBox::createKMessageBox(this, buttonBox, QMessageBox::Question, text, QStringList(), QString(), nullptr, KMessageBox::NoExec);
}

MDNAdviceDialog::~MDNAdviceDialog()
{
}

MessageComposer::MDNAdvice MDNAdviceDialog::result() const
{
    return m_result;
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
