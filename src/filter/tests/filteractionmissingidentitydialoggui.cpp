/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "../../../autotests/dummykernel.h"
#include "../filter/dialog/filteractionmissingidentitydialog.h"
#include "../kernel/mailkernel.h"
#include <QApplication>
#include <QStandardPaths>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    auto kernel = new DummyKernel(nullptr);
    CommonKernel->registerKernelIf(kernel); // register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel); // SettingsIf is used in FolderTreeWidget

    auto w = new MailCommon::FilterActionMissingIdentityDialog(QStringLiteral("filename"));
    w->exec();
    app.exec();
    delete w;
    return 0;
}
