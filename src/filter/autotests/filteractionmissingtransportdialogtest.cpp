/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingtransportdialogtest.h"
#include "../filter/dialog/filteractionmissingtransportdialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QStandardPaths>
#include <QTest>

#include <MailTransport/TransportComboBox>

FilterActionMissingTransportDialogTest::FilterActionMissingTransportDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

FilterActionMissingTransportDialogTest::~FilterActionMissingTransportDialogTest()
{
}

void FilterActionMissingTransportDialogTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionMissingTransportDialog dlg(QStringLiteral("filename"));
    QVERIFY(!dlg.windowTitle().isEmpty());
    QVERIFY(dlg.isModal());
    auto *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    auto *label = dlg.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    auto *mComboBoxTransport = dlg.findChild<MailTransport::TransportComboBox *>(QStringLiteral("comboboxtransport"));
    QVERIFY(mComboBoxTransport);
}

QTEST_MAIN(FilterActionMissingTransportDialogTest)
