/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractiondeletetest.h"
#include "../filteractions/filteractiondelete.h"
#include <QLabel>
#include <QTest>
FilterActionDeleteTest::FilterActionDeleteTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionDeleteTest::~FilterActionDeleteTest()
{
}

void FilterActionDeleteTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionDelete filter(nullptr);
    QWidget *w = filter.createParamWidget(nullptr);
    QVERIFY(w);

    auto *lab = dynamic_cast<QLabel *>(w);
    QVERIFY(lab);
    QCOMPARE(lab->objectName(), QStringLiteral("label_delete"));
}

void FilterActionDeleteTest::shouldReturnSieveValue()
{
    MailCommon::FilterActionDelete filter(nullptr);
    QCOMPARE(filter.sieveCode(), QStringLiteral("discard;"));
}

void FilterActionDeleteTest::shouldBeNotEmpty()
{
    MailCommon::FilterActionDelete filter;
    QVERIFY(!filter.isEmpty());
}

void FilterActionDeleteTest::shouldRequiresPart()
{
    MailCommon::FilterActionDelete filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::Envelope);
}

void FilterActionDeleteTest::shouldDeleteItem()
{
    MailCommon::FilterActionDelete filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, false);

    filter.argsFromString(QString());
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), false);
    QCOMPARE(context.deleteItem(), true);
    QCOMPARE(context.needsFlagStore(), false);
    QCOMPARE(context.needsFullPayload(), false);
}

QTEST_MAIN(FilterActionDeleteTest)
