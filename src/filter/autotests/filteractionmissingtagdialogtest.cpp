/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingtagdialogtest.h"
#include "../filter/dialog/filteractionmissingtagdialog.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>

FilterActionMissingTagDialogTest::FilterActionMissingTagDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
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

    auto *label = dlg.findChild<QLabel *>(QStringLiteral("tagnamelabel"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    label = dlg.findChild<QLabel *>(QStringLiteral("filtername"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto *mTagList = dlg.findChild<QListWidget *>(QStringLiteral("taglist"));
    QVERIFY(mTagList);

    auto *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    auto *user1Button = dlg.findChild<QPushButton *>(QStringLiteral("addtag"));
    QVERIFY(user1Button);
    QVERIFY(!user1Button->text().isEmpty());
}

QTEST_MAIN(FilterActionMissingTagDialogTest)
