/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionexectest.h"
#include "../filteractions/filteractionexec.h"
#include "../filteractions/filteractionwithurl.h"
#include <QTest>

#include <KUrlRequester>

FilterActionExecTest::FilterActionExecTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionExecTest::~FilterActionExecTest()
{
}

void FilterActionExecTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionExec filter;
    QWidget *w = filter.createParamWidget(nullptr);
    auto *requester = w->findChild<KUrlRequester *>(QStringLiteral("requester"));
    QVERIFY(requester);

    auto *helpButton = w->findChild<MailCommon::FilterActionWithUrlHelpButton *>(QStringLiteral("helpbutton"));
    QVERIFY(helpButton);
}

void FilterActionExecTest::shouldHaveRequirePart()
{
    MailCommon::FilterActionExec filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

QTEST_MAIN(FilterActionExecTest)
