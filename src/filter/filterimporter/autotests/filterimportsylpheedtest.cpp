/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#include "filterimportsylpheedtest.h"
#include "filter/mailfilter.h"
#include "filtertestkernel.h"
#include <AkonadiCore/qtest_akonadi.h>
#include <MailCommon/MailKernel>

void FilterImportSylpheedtest::initTestCase()
{
    AkonadiTest::checkTestIsIsolated();

    auto kernel = new FilterTestKernel(this);
    CommonKernel->registerKernelIf(kernel); // register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel); // SettingsIf is used in FolderTreeWidget
}

void FilterImportSylpheedtest::testImportFilters()
{
}

QTEST_AKONADIMAIN(FilterImportSylpheedtest)
