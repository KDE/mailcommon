/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QApplication>
#include "../filter/dialog/filteractionmissingsoundurldialog.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MailCommon::FilterActionMissingSoundUrlDialog *w = new MailCommon::FilterActionMissingSoundUrlDialog(QStringLiteral("filename"), QStringLiteral("argument"));
    w->exec();
    app.exec();
    delete w;
    return 0;
}
