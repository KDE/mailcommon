/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "snippetselectattachmentdialogtest.h"
#include "snippets/snippetselectattachmentdialog.h"
#include "snippets/snippetselectattachmentwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(SnippetSelectAttachmentDialogTest)

SnippetSelectAttachmentDialogTest::SnippetSelectAttachmentDialogTest(QObject *parent)
    : QObject(parent)
{

}

void SnippetSelectAttachmentDialogTest::shouldHaveDefaultValues()
{
    MailCommon::SnippetSelectAttachmentDialog dlg;
    QVERIFY(!dlg.windowTitle().isEmpty());

    QVBoxLayout *mainLayout = dlg.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    MailCommon::SnippetSelectorWidget *mAttachmentWidget = dlg.findChild<MailCommon::SnippetSelectorWidget *>(QStringLiteral("attachmentwidget"));
    QVERIFY(mAttachmentWidget);

    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), {QDialogButtonBox::Ok | QDialogButtonBox::Cancel});
}
