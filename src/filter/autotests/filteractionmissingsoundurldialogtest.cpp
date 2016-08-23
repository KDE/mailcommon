/*
   Copyright (C) 2016 Laurent Montel <montel@kde.org>

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

#include "filteractionmissingsoundurldialogtest.h"
#include "../filter/dialog/filteractionmissingsoundurldialog.h"
#include <QTest>
#include <QStandardPaths>
#include <QDialogButtonBox>
#include <QLabel>
#include <KUrlRequester>

FilterActionMissingSoundUrlDialogTest::FilterActionMissingSoundUrlDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

FilterActionMissingSoundUrlDialogTest::~FilterActionMissingSoundUrlDialogTest()
{

}

void FilterActionMissingSoundUrlDialogTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionMissingSoundUrlDialog dlg(QStringLiteral("filename"),QStringLiteral("arg"));
    QVERIFY(!dlg.windowTitle().isEmpty());

    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    QLabel *label = dlg.findChild<QLabel *>(QStringLiteral("oldlabel"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    label = dlg.findChild<QLabel *>(QStringLiteral("selectlabel"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    KUrlRequester *mUrlWidget = dlg.findChild<KUrlRequester *>(QStringLiteral("urlwidget"));
    QVERIFY(mUrlWidget);
}

QTEST_MAIN(FilterActionMissingSoundUrlDialogTest)
