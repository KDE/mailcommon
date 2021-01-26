/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "../filter/dialog/filteractionmissingtagdialog.h"
#include <QApplication>
#include <QMap>
#include <QUrl>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QMap<QUrl, QString> map;
    auto w = new MailCommon::FilterActionMissingTagDialog(map, QStringLiteral("filename"), QStringLiteral("argument"));
    w->exec();
    app.exec();
    delete w;
    return 0;
}
