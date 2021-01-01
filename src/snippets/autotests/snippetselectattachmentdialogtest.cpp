/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

    auto *mainLayout = dlg.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto *mAttachmentWidget = dlg.findChild<MailCommon::SnippetSelectAttachmentWidget *>(QStringLiteral("attachmentwidget"));
    QVERIFY(mAttachmentWidget);

    auto *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), {QDialogButtonBox::Ok | QDialogButtonBox::Cancel});
}
