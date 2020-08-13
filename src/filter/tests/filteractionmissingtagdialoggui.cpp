/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QApplication>
#include "../filter/dialog/filteractionmissingtagdialog.h"
#include <QMap>
#include <QUrl>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QMap<QUrl, QString> map;
    MailCommon::FilterActionMissingTagDialog *w = new MailCommon::FilterActionMissingTagDialog(map, QStringLiteral("filename"), QStringLiteral("argument"));
    w->exec();
    app.exec();
    delete w;
    return 0;
}
