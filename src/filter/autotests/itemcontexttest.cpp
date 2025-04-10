/*
  SPDX-FileCopyrightText: 2014-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "itemcontexttest.h"
#include "../itemcontext.h"
#include <Akonadi/Item>
#include <QTest>
ItemContextTest::ItemContextTest(QObject *parent)
    : QObject(parent)
{
}

ItemContextTest::~ItemContextTest() = default;

void ItemContextTest::shouldHaveDefaultValue()
{
    Akonadi::Item item(42);
    MailCommon::ItemContext itemContext(item, true);
    QCOMPARE(itemContext.item(), item);
    QVERIFY(!itemContext.moveTargetCollection().isValid());
    QVERIFY(!itemContext.needsPayloadStore());
    QVERIFY(!itemContext.needsFlagStore());
    QVERIFY(itemContext.needsFullPayload());
    QVERIFY(!itemContext.deleteItem());

    MailCommon::ItemContext itemContext2(item, false);
    QCOMPARE(itemContext2.item(), item);
    QVERIFY(!itemContext2.moveTargetCollection().isValid());
    QVERIFY(!itemContext2.needsPayloadStore());
    QVERIFY(!itemContext2.needsFlagStore());
    QVERIFY(!itemContext2.needsFullPayload());
    QVERIFY(!itemContext2.deleteItem());
}

void ItemContextTest::shouldAssignContext_data()
{
    QTest::addColumn<bool>("needspayloadstore");
    QTest::addColumn<bool>("needsflagstore");
    QTest::addColumn<bool>("needsfullpayload");
    QTest::addColumn<bool>("deleteitem");
    QTest::newRow("no boolean") << false << false << false << false;
    QTest::newRow("needspayloadstore") << true << false << false << false;
    QTest::newRow("needsflagstore") << false << true << false << false;
    QTest::newRow("needsfullpayload") << false << false << true << false;
    QTest::newRow("deleteitem") << false << false << false << true;
    QTest::newRow("all") << true << true << true << true;

    QTest::newRow("T T F F") << true << true << false << false;
    QTest::newRow("T T T F") << true << true << true << false;

    QTest::newRow("F T F F") << false << true << false << false;
    QTest::newRow("F F T F") << false << false << true << false;
    QTest::newRow("F T T F") << false << true << true << false;
    QTest::newRow("F T T T") << false << true << true << true;
    QTest::newRow("F F F T") << false << false << false << true;

    QTest::newRow("F T F T") << false << true << false << true;
    QTest::newRow("F F F T") << false << false << false << true;
}

void ItemContextTest::shouldAssignContext()
{
    QFETCH(bool, needspayloadstore);
    QFETCH(bool, needsflagstore);
    QFETCH(bool, needsfullpayload);
    QFETCH(bool, deleteitem);

    Akonadi::Item item(42);
    MailCommon::ItemContext itemContext(item, needsfullpayload);

    if (needspayloadstore) {
        itemContext.setNeedsPayloadStore();
    }
    if (needsflagstore) {
        itemContext.setNeedsFlagStore();
    }
    if (deleteitem) {
        itemContext.setDeleteItem();
    }
    QCOMPARE(itemContext.item(), item);
    QCOMPARE(itemContext.needsFlagStore(), needsflagstore);
    QCOMPARE(itemContext.needsFullPayload(), needsfullpayload);
    QCOMPARE(itemContext.needsPayloadStore(), needspayloadstore);
    QCOMPARE(itemContext.deleteItem(), deleteitem);
}

void ItemContextTest::shouldAssignCollection()
{
    Akonadi::Item item(42);
    Akonadi::Collection col(12);
    MailCommon::ItemContext itemContext(item, true);
    itemContext.setMoveTargetCollection(col);
    QCOMPARE(itemContext.item(), item);
    QCOMPARE(itemContext.moveTargetCollection(), col);
}

QTEST_MAIN(ItemContextTest)

#include "moc_itemcontexttest.cpp"
