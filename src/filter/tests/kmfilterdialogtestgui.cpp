/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "kmfilterdialogtestgui.h"
#include "../../../autotests/dummykernel.h"
#include "../kernel/mailkernel.h"
#include "../kmfilterdialog.h"

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    auto kernel = new DummyKernel(nullptr);
    CommonKernel->registerKernelIf(kernel); // register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel); // SettingsIf is used in FolderTreeWidget

    QList<KActionCollection *> lstAction;
    auto dlg = new MailCommon::KMFilterDialog(lstAction, nullptr, true);
    dlg->resize(800, 600);
    dlg->show();
    const int ret = app.exec();
    return ret;
}
