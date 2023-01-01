/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#include "filterimportergmailtest.h"
#include "filter/mailfilter.h"
#include "filtertestkernel.h"
#include <MailCommon/MailKernel>
#include <akonadi/qtest_akonadi.h>

FilterImporterGmailTest::FilterImporterGmailTest(QObject *parent)
    : QObject(parent)
{
}

void FilterImporterGmailTest::initTestCase()
{
    AkonadiTest::checkTestIsIsolated();

    auto kernel = new FilterTestKernel(this);
    CommonKernel->registerKernelIf(kernel); // register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel); // SettingsIf is used in FolderTreeWidget
}

void FilterImporterGmailTest::testImportFilters()
{
}

QTEST_AKONADIMAIN(FilterImporterGmailTest)
