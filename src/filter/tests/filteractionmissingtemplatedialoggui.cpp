/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QApplication>
#include "../filter/dialog/filteractionmissingtemplatedialog.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MailCommon::FilterActionMissingTemplateDialog *w = new MailCommon::FilterActionMissingTemplateDialog(QStringList(), QStringLiteral("argument"));
    w->exec();
    app.exec();
    delete w;
    return 0;
}
