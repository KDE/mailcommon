/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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
