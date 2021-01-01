/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingaccountdialogtest.h"
#include "../filter/dialog/filteractionmissingaccountdialog.h"
#include "../filter/kmfilteraccountlist.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QStandardPaths>
#include <QTest>

FilterActionMissingAccountDialogTest::FilterActionMissingAccountDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

FilterActionMissingAccountDialogTest::~FilterActionMissingAccountDialogTest()
{
}

void FilterActionMissingAccountDialogTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionMissingAccountDialog dlg(QStringList(), QStringLiteral("filename"));
    QVERIFY(!dlg.windowTitle().isEmpty());
    QVERIFY(dlg.isModal());

    auto *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    auto *label = dlg.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto *mAccountList = dlg.findChild<MailCommon::KMFilterAccountList *>(QStringLiteral("accountlist"));
    QVERIFY(mAccountList);
}

QTEST_MAIN(FilterActionMissingAccountDialogTest)
