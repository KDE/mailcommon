#include "filteractionsettransporttest.h"
#include "../filteractions/filteractionsettransport.h"
#include <MailTransport/TransportComboBox>
#include <QTest>

FilterActionSetTransportTest::FilterActionSetTransportTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionSetTransportTest::~FilterActionSetTransportTest()
{
}

void FilterActionSetTransportTest::shouldBeEmpty()
{
    MailCommon::FilterActionSetTransport filter;
    QVERIFY(filter.isEmpty());
}

void FilterActionSetTransportTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionSetTransport filter;
    QWidget *w = filter.createParamWidget(nullptr);
    QCOMPARE(w->objectName(), QStringLiteral("transportcombobox"));

    auto *transportCombobox = dynamic_cast<MailTransport::TransportComboBox *>(w);
    QVERIFY(transportCombobox);
}

void FilterActionSetTransportTest::shouldHaveRequiredPart()
{
    MailCommon::FilterActionSetTransport filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

void FilterActionSetTransportTest::shouldHaveSieveRequires()
{
    MailCommon::FilterActionSetTransport filter;
    QCOMPARE(filter.sieveRequires(), QStringList());
}

QTEST_MAIN(FilterActionSetTransportTest)
