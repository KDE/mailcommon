/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "../filter/dialog/filteractionmissingaccountdialog.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    auto w = new MailCommon::FilterActionMissingAccountDialog(QStringList(), QStringLiteral("filename"));
    w->exec();
    app.exec();
    delete w;
    return 0;
}
