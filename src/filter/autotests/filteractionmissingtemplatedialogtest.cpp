/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingtemplatedialogtest.h"
#include "../filter/dialog/filteractionmissingtemplatedialog.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QStandardPaths>
#include <QTest>

FilterActionMissingTemplateDialogTest::FilterActionMissingTemplateDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

FilterActionMissingTemplateDialogTest::~FilterActionMissingTemplateDialogTest()
{
}

void FilterActionMissingTemplateDialogTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionMissingTemplateDialog dlg(QStringList(), QStringLiteral("filename"));
    QVERIFY(!dlg.windowTitle().isEmpty());

    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    QLabel *label = dlg.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    QComboBox *mComboBoxTemplate = dlg.findChild<QComboBox *>(QStringLiteral("comboboxtemplate"));
    QVERIFY(mComboBoxTemplate);
}

QTEST_MAIN(FilterActionMissingTemplateDialogTest)
