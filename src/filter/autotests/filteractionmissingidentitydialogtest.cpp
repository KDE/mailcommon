/*
   Copyright (C) 2016-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "filteractionmissingidentitydialogtest.h"
#include "../filter/dialog/filteractionmissingidentitydialog.h"
#include "../../autotests/dummykernel.h"
#include "../../kernel/mailkernel.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QTest>
#include <KIdentityManagement/IdentityCombo>

FilterActionMissingIdentityDialogTest::FilterActionMissingIdentityDialogTest(QObject *parent)
    : QObject(parent)
{
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
