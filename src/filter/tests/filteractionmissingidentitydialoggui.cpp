/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QApplication>
#include <QStandardPaths>
#include "../../../autotests/dummykernel.h"
#include "../kernel/mailkernel.h"
#include "../filter/dialog/filteractionmissingidentitydialog.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    DummyKernel *kernel = new DummyKernel(nullptr);
    CommonKernel->registerKernelIf(kernel);   //register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel);   //SettingsIf is used in FolderTreeWidget

    MailCommon::FilterActionMissingIdentityDialog *w = new MailCommon::FilterActionMissingIdentityDialog(QStringLiteral("filename"));
    w->exec();
    app.exec();
    delete w;
    return 0;
}
