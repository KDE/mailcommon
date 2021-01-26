/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingidentitydialogtest.h"
#include "../../autotests/dummykernel.h"
#include "../../kernel/mailkernel.h"
#include "../filter/dialog/filteractionmissingidentitydialog.h"
#include <KIdentityManagement/IdentityCombo>
#include <QDialogButtonBox>
#include <QLabel>
#include <QStandardPaths>
#include <QTest>

FilterActionMissingIdentityDialogTest::FilterActionMissingIdentityDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

FilterActionMissingIdentityDialogTest::~FilterActionMissingIdentityDialogTest()
{
}

void FilterActionMissingIdentityDialogTest::initTestCase()
{
    auto kernel = new DummyKernel(nullptr);
    CommonKernel->registerKernelIf(kernel); // register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel); // SettingsIf is used in FolderTreeWidget
}

void FilterActionMissingIdentityDialogTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionMissingIdentityDialog dlg(QStringLiteral("filename"));
    QVERIFY(dlg.isModal());
    QVERIFY(!dlg.windowTitle().isEmpty());

    auto *label = dlg.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto *mComboBoxIdentity = dlg.findChild<KIdentityManagement::IdentityCombo *>(QStringLiteral("comboboxidentity"));
    QVERIFY(mComboBoxIdentity);

    auto *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);
}

QTEST_MAIN(FilterActionMissingIdentityDialogTest)
