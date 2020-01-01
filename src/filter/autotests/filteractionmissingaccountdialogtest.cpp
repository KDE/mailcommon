/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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

    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    QLabel *label = dlg.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    MailCommon::KMFilterAccountList *mAccountList = dlg.findChild<MailCommon::KMFilterAccountList *>(QStringLiteral("accountlist"));
    QVERIFY(mAccountList);
}

QTEST_MAIN(FilterActionMissingAccountDialogTest)
