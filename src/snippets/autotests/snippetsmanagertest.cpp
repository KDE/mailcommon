/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "snippetsmanagertest.h"
#include "../snippetsmanager.h"
#include <KActionCollection>
#include <QTest>

SnippetsManagerTest::SnippetsManagerTest(QObject *parent)
    : QObject(parent)
{
}

SnippetsManagerTest::~SnippetsManagerTest()
{
}

void SnippetsManagerTest::shouldHaveDefaultValue()
{
    auto collection = new KActionCollection(this);
    MailCommon::SnippetsManager snippetManager(collection, this);
    QVERIFY(snippetManager.deleteSnippetAction());
    QVERIFY(snippetManager.model());
    QVERIFY(snippetManager.selectionModel());
    QVERIFY(snippetManager.addSnippetAction());
    QVERIFY(snippetManager.editSnippetAction());
    QVERIFY(snippetManager.deleteSnippetAction());
    QVERIFY(snippetManager.addSnippetGroupAction());
    QVERIFY(snippetManager.editSnippetGroupAction());
    QVERIFY(snippetManager.deleteSnippetGroupAction());
    QVERIFY(snippetManager.insertSnippetAction());
    QVERIFY(snippetManager.selectedName().isEmpty());
}

QTEST_MAIN(SnippetsManagerTest)
