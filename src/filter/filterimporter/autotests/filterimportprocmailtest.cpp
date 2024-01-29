/*
  SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#include "filterimportprocmailtest.h"
#include "../filterimporterprocmail.h"
#include "filter/mailfilter.h"
#include "filtertestkernel.h"
#include <MailCommon/MailKernel>
#include <akonadi/qtest_akonadi.h>

void FilterImportProcmailtest::initTestCase()
{
    AkonadiTest::checkTestIsIsolated();

    auto kernel = new FilterTestKernel(this);
    CommonKernel->registerKernelIf(kernel); // register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel); // SettingsIf is used in FolderTreeWidget
}

void FilterImportProcmailtest::testImportFilters()
{
#if 0
    const QString filter = QLatin1StringView("version=\"9\"\n"
                                         "logging=\"no\"\n"
                                         "name=\"Match All Messages\"\n"
                                         "enabled=\"yes\"\n"
                                         "type=\"17\"\n"
                                         "action=\"Mark read\"\n"
                                         "condition=\"ALL\"\n");
    MailCommon::FilterImporterProcmail importer(filter, false);
    QList<MailCommon::MailFilter *> lst = importer.importFilter();
    QCOMPARE(lst.count(), 1);
    MailCommon::MailFilter *f = lst.at(0);
    QVERIFY(f->isEnabled());
    QCOMPARE(f->name(), QStringLiteral("Match All Messages"));
    QCOMPARE(f->pattern()->op(), MailCommon::SearchPattern::OpAll);

    qDeleteAll(lst);
#endif
}

QTEST_AKONADIMAIN(FilterImportProcmailtest)

#include "moc_filterimportprocmailtest.cpp"
