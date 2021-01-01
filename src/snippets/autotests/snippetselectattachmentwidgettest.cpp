/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "snippetselectattachmentwidgettest.h"
#include "snippets/snippetselectattachmentwidget.h"
#include <PimCommon/SimpleStringListEditor>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(SnippetSelectAttachmentWidgetTest)
SnippetSelectAttachmentWidgetTest::SnippetSelectAttachmentWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void SnippetSelectAttachmentWidgetTest::shouldHaveDefaultValues()
{
    MailCommon::SnippetSelectAttachmentWidget w;
    auto *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto *mEditor = w.findChild<PimCommon::SimpleStringListEditor *>(QStringLiteral("editor"));
    QVERIFY(mEditor);
}
