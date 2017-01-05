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

#include "filteractionmissingtemplatedialogtest.h"
#include "../filter/dialog/filteractionmissingtemplatedialog.h"
#include <KComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QTest>

FilterActionMissingTemplateDialogTest::FilterActionMissingTemplateDialogTest(QObject *parent)
    : QObject(parent)
{

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

    KComboBox *mComboBoxTemplate = dlg.findChild<KComboBox *>(QStringLiteral("comboboxtemplate"));
    QVERIFY(mComboBoxTemplate);
}

QTEST_MAIN(FilterActionMissingTemplateDialogTest)
