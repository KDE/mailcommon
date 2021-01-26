/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#include "filterimportclawmailtest.h"
#include "../filterimporterclawsmail.h"
#include "filter/mailfilter.h"
#include "filtertestkernel.h"
#include <AkonadiCore/qtest_akonadi.h>
#include <MailCommon/MailKernel>

void FilterImportClawMailtest::initTestCase()
{
    AkonadiTest::checkTestIsIsolated();

    auto kernel = new FilterTestKernel(this);
    CommonKernel->registerKernelIf(kernel); // register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel); // SettingsIf is used in FolderTreeWidget
}

void FilterImportClawMailtest::testImportFilters()
{
    MailCommon::FilterImporterClawsMails importer;
    MailCommon::MailFilter *filter =
        importer.parseLine(QStringLiteral("enabled rulename \"foo\" subject matchcase \"fff\" add_to_addressbook \"From\" \"addrbook-000002.xml\""));
    QCOMPARE(filter->toolbarName(), QStringLiteral("foo"));
    QVERIFY(filter->isEnabled());
    delete filter;
}

QTEST_AKONADIMAIN(FilterImportClawMailtest)
