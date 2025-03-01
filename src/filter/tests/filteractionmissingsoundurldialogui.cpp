/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "../filter/dialog/filteractionmissingsoundurldialog.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    auto w = new MailCommon::FilterActionMissingSoundUrlDialog(QStringLiteral("filename"), QStringLiteral("argument"));
    w->exec();
    app.exec();
    delete w;
    return 0;
}
