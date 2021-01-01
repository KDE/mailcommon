/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "snippetcustomfileattachmentnamedialogtest.h"
#include "snippets/snippetcustomfileattachmentnamedialog.h"
#include "snippets/snippetcustomfileattachmentnamewidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(SnippetCustomFileAttachmentNameDialogTest)

SnippetCustomFileAttachmentNameDialogTest::SnippetCustomFileAttachmentNameDialogTest(QObject *parent)
    : QObject(parent)
{
}

void SnippetCustomFileAttachmentNameDialogTest::shouldHaveDefaultValues()
{
    MailCommon::SnippetCustomFileAttachmentNameDialog dlg;
    auto *mainLayout = dlg.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    QVERIFY(!dlg.windowTitle().isEmpty());

    auto *mCustomWidget = dlg.findChild<MailCommon::SnippetCustomFileAttachmentNameWidget *>(QStringLiteral("mCustomWidget"));
    QVERIFY(mCustomWidget);
    mainLayout->addWidget(mCustomWidget);

    auto *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), {QDialogButtonBox::Ok | QDialogButtonBox::Cancel});
}
