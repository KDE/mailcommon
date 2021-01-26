/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingsoundurldialogtest.h"
#include "../filter/dialog/filteractionmissingsoundurldialog.h"
#include <KUrlRequester>
#include <QDialogButtonBox>
#include <QLabel>
#include <QStandardPaths>
#include <QTest>

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
    MailCommon::FilterActionMissingSoundUrlDialog dlg(QStringLiteral("filename"), QStringLiteral("arg"));
    QVERIFY(!dlg.windowTitle().isEmpty());

    auto *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    auto *label = dlg.findChild<QLabel *>(QStringLiteral("oldlabel"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    label = dlg.findChild<QLabel *>(QStringLiteral("selectlabel"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto *mUrlWidget = dlg.findChild<KUrlRequester *>(QStringLiteral("urlwidget"));
    QVERIFY(mUrlWidget);
}

QTEST_MAIN(FilterActionMissingSoundUrlDialogTest)
