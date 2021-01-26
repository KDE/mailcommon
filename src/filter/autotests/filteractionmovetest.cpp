/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionmovetest.h"
#include "../filteractions/filteractionmove.h"
#include "folder/folderrequester.h"
#include <QTest>

FilterActionMoveTest::FilterActionMoveTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionMoveTest::~FilterActionMoveTest()
{
}

void FilterActionMoveTest::shouldHaveSieveRequires()
{
    MailCommon::FilterActionMove filter;
    QCOMPARE(filter.sieveRequires(), QStringList() << QStringLiteral("fileinto"));
}

void FilterActionMoveTest::shouldHaveRequiresPart()
{
    MailCommon::FilterActionMove filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::Envelope);
}

void FilterActionMoveTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionMove filter;
    QWidget *w = filter.createParamWidget(nullptr);
    QVERIFY(w);
    auto *requester = dynamic_cast<MailCommon::FolderRequester *>(w);
    QVERIFY(requester);
    QCOMPARE(requester->objectName(), QStringLiteral("folderrequester"));
}

QTEST_MAIN(FilterActionMoveTest)
