/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "snippetwidgettest.h"
#include "snippets/snippetwidget.h"
#include <QTest>
QTEST_MAIN(SnippetWidgetTest)

SnippetWidgetTest::SnippetWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void SnippetWidgetTest::shouldHaveDefaultValues()
{
    MailCommon::SnippetWidget w;
    QVERIFY(w.name().isEmpty());
    QVERIFY(w.text().isEmpty());
    QVERIFY(w.keySequence().isEmpty());
    QVERIFY(w.subject().isEmpty());
    QVERIFY(w.keyword().isEmpty());
    QVERIFY(w.to().isEmpty());
    QVERIFY(w.cc().isEmpty());
    QVERIFY(w.bcc().isEmpty());
    QVERIFY(!w.snippetIsValid());
    QVERIFY(!w.isGroupSelected());
    QVERIFY(!w.wasChanged());
}

void SnippetWidgetTest::shouldClearWidget()
{
    MailCommon::SnippetWidget w;
    w.setName(QStringLiteral("bla"));
    w.setText(QStringLiteral("bli"));
    w.setSubject(QStringLiteral("zz"));
    w.setKeyword(QStringLiteral("zzsss"));
    w.setTo(QStringLiteral("to"));
    w.setCc(QStringLiteral("tos"));
    w.setBcc(QStringLiteral("bcc"));
    QVERIFY(!w.name().isEmpty());
    QVERIFY(!w.text().isEmpty());
    QVERIFY(!w.subject().isEmpty());
    QVERIFY(!w.keyword().isEmpty());
    QVERIFY(!w.to().isEmpty());
    QVERIFY(!w.cc().isEmpty());
    QVERIFY(!w.bcc().isEmpty());
    w.clear();

    QVERIFY(w.name().isEmpty());
    QVERIFY(w.text().isEmpty());
    QVERIFY(w.keySequence().isEmpty());
    QVERIFY(w.subject().isEmpty());
    QVERIFY(w.keyword().isEmpty());
    QVERIFY(w.to().isEmpty());
    QVERIFY(w.cc().isEmpty());
    QVERIFY(w.bcc().isEmpty());
    QVERIFY(!w.snippetIsValid());
    QVERIFY(!w.isGroupSelected());
    QVERIFY(w.wasChanged());
}
