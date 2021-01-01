/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "snippetattachmentwidgettest.h"
#include "snippets/snippetattachmentwidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTest>
#include <QToolButton>
QTEST_MAIN(SnippetAttachmentWidgetTest)

SnippetAttachmentWidgetTest::SnippetAttachmentWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void SnippetAttachmentWidgetTest::shouldHaveDefaultValues()
{
    MailCommon::SnippetAttachmentWidget w;

    auto *layout = w.findChild<QHBoxLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto *mLineEdit = w.findChild<QLineEdit *>(QStringLiteral("lineedit"));
    QVERIFY(mLineEdit);
    QVERIFY(mLineEdit->text().isEmpty());
    QVERIFY(mLineEdit->isReadOnly());
    QVERIFY(!mLineEdit->placeholderText().isEmpty());

    auto *button = w.findChild<QToolButton *>(QStringLiteral("button"));
    QVERIFY(button);
    QVERIFY(!button->text().isEmpty());
    QVERIFY(!button->toolTip().isEmpty());
}
