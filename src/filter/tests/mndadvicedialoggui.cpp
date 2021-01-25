/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filter/mdnadvicedialog.h"

#include <QApplication>
#include <QDebug>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    auto w = new MailCommon::MDNAdviceDialog(QStringLiteral("test mnda"), true);
    MessageComposer::MDNAdvice rc = MessageComposer::MDNIgnore;
    if (w->exec()) {
        rc = w->result();
    }
    switch (rc) {
    case MessageComposer::MDNSend:
        qDebug() << "MessageComposer::MDNSend ";
        break;
    case MessageComposer::MDNSendDenied:
        qDebug() << "MessageComposer::MDNSendDenied ";
        break;
    case MessageComposer::MDNIgnore:
        qDebug() << "MessageComposer::MDNIgnore ";
        break;
    }
    app.exec();
    delete w;
    return 0;
}
