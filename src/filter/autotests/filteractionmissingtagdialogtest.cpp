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

#include "filteractionmissingtagdialogtest.h"
#include "../filter/dialog/filteractionmissingtagdialog.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QTest>

FilterActionMissingTagDialogTest::FilterActionMissingTagDialogTest(QObject *parent)
    : QObject(parent)
{

}

FilterActionMissingTagDialogTest::~FilterActionMissingTagDialogTest()
{

}

void FilterActionMissingTagDialogTest::shouldHaveDefaultValue()
{
    QMap<QUrl, QString> map;
    MailCommon::FilterActionMissingTagDialog dlg(map, QStringLiteral("filename"), QStringLiteral("argument"));
    QVERIFY(dlg.isModal());
    QVERIFY(!dlg.windowTitle().isEmpty());

    QLabel *label = dlg.findChild<QLabel *>(QStringLiteral("tagnamelabel"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    label = dlg.findChild<QLabel *>(QStringLiteral("filtername"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    QListWidget *mTagList = dlg.findChild<QListWidget *>(QStringLiteral("taglist"));
    QVERIFY(mTagList);

    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    QPushButton *user1Button = dlg.findChild<QPushButton *>(QStringLiteral("addtag"));
    QVERIFY(user1Button);
    QVERIFY(!user1Button->text().isEmpty());
}

QTEST_MAIN(FilterActionMissingTagDialogTest)
