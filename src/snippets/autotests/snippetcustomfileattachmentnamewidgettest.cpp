/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "snippetcustomfileattachmentnamewidgettest.h"
#include "snippets/snippetcustomfileattachmentnamewidget.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(SnippetCustomFileAttachmentNameWidgetTest)

SnippetCustomFileAttachmentNameWidgetTest::SnippetCustomFileAttachmentNameWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void SnippetCustomFileAttachmentNameWidgetTest::shouldHaveDefaultValues()
{
    MailCommon::SnippetCustomFileAttachmentNameWidget w;
    auto *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto *mLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mLineEdit"));
    QVERIFY(mLineEdit);
    QVERIFY(mLineEdit->text().isEmpty());

    auto *selectVariable = w.findChild<QPushButton *>(QStringLiteral("selectVariable"));
    QVERIFY(selectVariable);
    QVERIFY(!selectVariable->text().isEmpty());
    QVERIFY(selectVariable->menu());
}
