/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionwithaddresstest.h"
#include "../filteractions/filteractionwithaddress.h"
#include <Akonadi/EmailAddressRequester>
#include <QTest>
#include <QWidget>

class TestFilterActionWithAddress : public MailCommon::FilterActionWithAddress
{
public:
    TestFilterActionWithAddress()
        : MailCommon::FilterActionWithAddress(QStringLiteral("foo"), QStringLiteral("bla"))
    {
    }

    FilterAction::ReturnCode process(MailCommon::ItemContext &, bool) const override
    {
        return GoOn;
    }

    Q_REQUIRED_RESULT MailCommon::SearchRule::RequiredPart requiredPart() const override
    {
        return MailCommon::SearchRule::CompleteMessage;
    }
};

FilterActionWithAddressTest::FilterActionWithAddressTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionWithAddressTest::~FilterActionWithAddressTest() = default;

void FilterActionWithAddressTest::shouldHaveDefaultValue()
{
    TestFilterActionWithAddress filter;
    QWidget *w = filter.createParamWidget(nullptr);
    QCOMPARE(w->objectName(), QStringLiteral("emailaddressrequester"));
    auto requester = dynamic_cast<Akonadi::EmailAddressRequester *>(w);
    QVERIFY(requester);
    QVERIFY(filter.isEmpty());
}

void FilterActionWithAddressTest::shouldAssignValue()
{
    TestFilterActionWithAddress filter;
    QWidget *w = filter.createParamWidget(nullptr);
    auto requester = dynamic_cast<Akonadi::EmailAddressRequester *>(w);
    filter.argsFromString(QStringLiteral("foo"));
    filter.setParamWidgetValue(w);
    QVERIFY(!filter.isEmpty());
    QVERIFY(!requester->text().isEmpty());
}

void FilterActionWithAddressTest::shouldRequiresPart()
{
    TestFilterActionWithAddress filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

QTEST_MAIN(FilterActionWithAddressTest)

#include "moc_filteractionwithaddresstest.cpp"
