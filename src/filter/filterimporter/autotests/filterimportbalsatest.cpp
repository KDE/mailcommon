/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#include "filterimportbalsatest.h"
#include "filter/mailfilter.h"
#include "filtertestkernel.h"
#include <MailCommon/MailKernel>
#include <AkonadiCore/qtest_akonadi.h>

void FilterImportBalsatest::initTestCase()
{
    AkonadiTest::checkTestIsIsolated();

    FilterTestKernel *kernel = new FilterTestKernel(this);
    CommonKernel->registerKernelIf(kernel);   //register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel);   //SettingsIf is used in FolderTreeWidget
}

void FilterImportBalsatest::testImportFilters()
{
}

QTEST_AKONADIMAIN(FilterImportBalsatest)
