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

#include "filteractionmissingtransportdialogtest.h"
#include "../filter/dialog/filteractionmissingtransportdialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QTest>
#include <QStandardPaths>

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
    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    QLabel *label = dlg.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    MailTransport::TransportComboBox *mComboBoxTransport = dlg.findChild<MailTransport::TransportComboBox *>(QStringLiteral("comboboxtransport"));
    QVERIFY(mComboBoxTransport);
}

QTEST_MAIN(FilterActionMissingTransportDialogTest)
