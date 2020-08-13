/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingidentitydialogtest.h"
#include "../filter/dialog/filteractionmissingidentitydialog.h"
#include "../../autotests/dummykernel.h"
#include "../../kernel/mailkernel.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QLabel>
#include <QTest>
#include <KIdentityManagement/IdentityCombo>

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
    DummyKernel *kernel = new DummyKernel(nullptr);
    CommonKernel->registerKernelIf(kernel);   //register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel);   //SettingsIf is used in FolderTreeWidget
}

void FilterActionMissingIdentityDialogTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionMissingIdentityDialog dlg(QStringLiteral("filename"));
    QVERIFY(dlg.isModal());
    QVERIFY(!dlg.windowTitle().isEmpty());

    QLabel *label = dlg.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    KIdentityManagement::IdentityCombo *mComboBoxIdentity = dlg.findChild<KIdentityManagement::IdentityCombo *>(QStringLiteral("comboboxidentity"));
    QVERIFY(mComboBoxIdentity);

    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);
}

QTEST_MAIN(FilterActionMissingIdentityDialogTest)
