/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionexectest.h"
#include <QTest>
#include "../filteractions/filteractionexec.h"
#include "../filteractions/filteractionwithurl.h"

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
    KUrlRequester *requester = w->findChild<KUrlRequester *>(QStringLiteral("requester"));
    QVERIFY(requester);

    MailCommon::FilterActionWithUrlHelpButton *helpButton = w->findChild<MailCommon::FilterActionWithUrlHelpButton *>(QStringLiteral("helpbutton"));
    QVERIFY(helpButton);
}

void FilterActionExecTest::shouldHaveRequirePart()
{
    MailCommon::FilterActionExec filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

QTEST_MAIN(FilterActionExecTest)
