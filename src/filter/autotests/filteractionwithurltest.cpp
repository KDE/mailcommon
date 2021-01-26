/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionwithurltest.h"
#include "../filteractions/filteractionwithurl.h"
#include <KLineEdit>
#include <KUrlRequester>
#include <QTest>

class TestFilterActionWithUrl : public MailCommon::FilterActionWithUrl
{
public:
    TestFilterActionWithUrl()
        : MailCommon::FilterActionWithUrl(QStringLiteral("test"), QStringLiteral("label"))
    {
    }

    FilterAction::ReturnCode process(MailCommon::ItemContext &, bool) const override
    {
        return GoOn;
    }

    MailCommon::SearchRule::RequiredPart requiredPart() const override
    {
        return MailCommon::SearchRule::CompleteMessage;
    }
};

FilterActionWithUrlTest::FilterActionWithUrlTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionWithUrlTest::~FilterActionWithUrlTest()
{
}

void FilterActionWithUrlTest::shouldHaveDefaultValue()
{
    TestFilterActionWithUrl filter;
    QWidget *w = filter.createParamWidget(nullptr);
    auto *requester = w->findChild<KUrlRequester *>(QStringLiteral("requester"));
    QVERIFY(requester);
    auto *toolButton = w->findChild<QToolButton *>(QStringLiteral("helpbutton"));
    QVERIFY(toolButton);
}

void FilterActionWithUrlTest::shouldClearWidget()
{
    TestFilterActionWithUrl filter;
    QWidget *w = filter.createParamWidget(nullptr);
    auto *requester = w->findChild<KUrlRequester *>(QStringLiteral("requester"));
    requester->setUrl(QUrl::fromLocalFile(QStringLiteral("/foo/bla")));
    QVERIFY(!requester->url().isEmpty());
    filter.clearParamWidget(w);
    QVERIFY(requester->url().isEmpty());
}

void FilterActionWithUrlTest::shouldAddValue()
{
    TestFilterActionWithUrl filter;
    QWidget *w = filter.createParamWidget(nullptr);
    auto *requester = w->findChild<KUrlRequester *>(QStringLiteral("requester"));
    filter.argsFromString(QStringLiteral("/foo"));
    filter.setParamWidgetValue(w);
    QCOMPARE(requester->lineEdit()->text(), QStringLiteral("/foo"));
}

void FilterActionWithUrlTest::shouldApplyValue()
{
    TestFilterActionWithUrl filter;
    QWidget *w = filter.createParamWidget(nullptr);
    filter.argsFromString(QStringLiteral("foo"));
    filter.setParamWidgetValue(w);
    filter.applyParamWidgetValue(w);
    QCOMPARE(filter.argsAsString(), QStringLiteral("foo"));
}

void FilterActionWithUrlTest::shouldTestUrl_data()
{
    QTest::addColumn<QString>("urlstr");
    QTest::addColumn<QString>("output");
    QTest::newRow("fullpath") << QStringLiteral("/usr/bin/ls") << QStringLiteral("/usr/bin/ls");
    QTest::newRow("local") << QStringLiteral("ls") << QStringLiteral("ls");
    QTest::newRow("localwithargument") << QStringLiteral("ls -l") << QStringLiteral("ls -l");
    QTest::newRow("fullpathwithargument") << QStringLiteral("/usr/bin/ls -l") << QStringLiteral("/usr/bin/ls -l");
    QTest::newRow("url") << QStringLiteral("file:///usr/bin/ls -l") << QStringLiteral("file:///usr/bin/ls -l");
    QTest::newRow("url2") << QStringLiteral("/usr/bin/ls -l") << QStringLiteral("/usr/bin/ls -l");
}

void FilterActionWithUrlTest::shouldTestUrl()
{
    QFETCH(QString, urlstr);
    QFETCH(QString, output);
    TestFilterActionWithUrl filter;
    QWidget *w = filter.createParamWidget(nullptr);
    filter.argsFromString(urlstr);
    filter.setParamWidgetValue(w);
    filter.applyParamWidgetValue(w);
    QCOMPARE(filter.argsAsString(), output);
}

QTEST_MAIN(FilterActionWithUrlTest)
