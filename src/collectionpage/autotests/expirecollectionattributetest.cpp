/*
  SPDX-FileCopyrightText: 2014-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "expirecollectionattributetest.h"
#include "../attributes/expirecollectionattribute.h"
#include <QTest>

Q_DECLARE_METATYPE(MailCommon::ExpireCollectionAttribute::ExpireUnits)
Q_DECLARE_METATYPE(MailCommon::ExpireCollectionAttribute::ExpireAction)
ExpireCollectionAttributeTest::ExpireCollectionAttributeTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<MailCommon::ExpireCollectionAttribute::ExpireUnits>();
    qRegisterMetaType<MailCommon::ExpireCollectionAttribute::ExpireAction>();
}

ExpireCollectionAttributeTest::~ExpireCollectionAttributeTest() = default;

void ExpireCollectionAttributeTest::shouldHaveDefaultValue()
{
    MailCommon::ExpireCollectionAttribute attr;
    QVERIFY(!attr.isAutoExpire());
    QCOMPARE(attr.unreadExpireAge(), 28);
    QCOMPARE(attr.readExpireAge(), 14);
    QCOMPARE(attr.expireAction(), MailCommon::ExpireCollectionAttribute::ExpireDelete);
    QCOMPARE(attr.unreadExpireUnits(), MailCommon::ExpireCollectionAttribute::ExpireNever);
    QCOMPARE(attr.readExpireUnits(), MailCommon::ExpireCollectionAttribute::ExpireNever);
    QCOMPARE(attr.expireToFolderId(), (qint64)-1);
}

void ExpireCollectionAttributeTest::shouldAssignValue_data()
{
    QTest::addColumn<bool>("autoexpire");
    QTest::addColumn<int>("unreadexpireage");
    QTest::addColumn<int>("readexpireage");
    QTest::addColumn<MailCommon::ExpireCollectionAttribute::ExpireUnits>("unreadexpireunit");
    QTest::addColumn<MailCommon::ExpireCollectionAttribute::ExpireUnits>("readexpireunit");
    QTest::addColumn<MailCommon::ExpireCollectionAttribute::ExpireAction>("expireaction");
    QTest::addColumn<Akonadi::Collection::Id>("akonadiid");

    QTest::newRow("add autoexpire") << true << 28 << 14 << MailCommon::ExpireCollectionAttribute::ExpireNever
                                    << MailCommon::ExpireCollectionAttribute::ExpireNever << MailCommon::ExpireCollectionAttribute::ExpireDelete << qint64(5);
    QTest::newRow("change action") << true << 28 << 14 << MailCommon::ExpireCollectionAttribute::ExpireNever
                                   << MailCommon::ExpireCollectionAttribute::ExpireNever << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(5);
    QTest::newRow("change age") << true << 150 << 1 << MailCommon::ExpireCollectionAttribute::ExpireNever << MailCommon::ExpireCollectionAttribute::ExpireNever
                                << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(5);
    QTest::newRow("change units") << true << 15 << 1 << MailCommon::ExpireCollectionAttribute::ExpireDays << MailCommon::ExpireCollectionAttribute::ExpireDays
                                  << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(5);
    QTest::newRow("change units2") << true << 150 << 18 << MailCommon::ExpireCollectionAttribute::ExpireMonths
                                   << MailCommon::ExpireCollectionAttribute::ExpireWeeks << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(6);
}

void ExpireCollectionAttributeTest::shouldAssignValue()
{
    QFETCH(bool, autoexpire);
    QFETCH(int, unreadexpireage);
    QFETCH(int, readexpireage);
    QFETCH(MailCommon::ExpireCollectionAttribute::ExpireUnits, unreadexpireunit);
    QFETCH(MailCommon::ExpireCollectionAttribute::ExpireUnits, readexpireunit);
    QFETCH(MailCommon::ExpireCollectionAttribute::ExpireAction, expireaction);
    QFETCH(Akonadi::Collection::Id, akonadiid);
    MailCommon::ExpireCollectionAttribute attr;
    attr.setAutoExpire(autoexpire);
    attr.setUnreadExpireAge(unreadexpireage);
    attr.setReadExpireAge(readexpireage);
    attr.setUnreadExpireUnits(unreadexpireunit);
    attr.setReadExpireUnits(readexpireunit);
    attr.setExpireAction(expireaction);
    attr.setExpireToFolderId(akonadiid);

    QCOMPARE(attr.isAutoExpire(), autoexpire);
    QCOMPARE(attr.unreadExpireAge(), unreadexpireage);
    QCOMPARE(attr.readExpireAge(), readexpireage);
    QCOMPARE(attr.expireAction(), expireaction);
    QCOMPARE(attr.unreadExpireUnits(), unreadexpireunit);
    QCOMPARE(attr.readExpireUnits(), readexpireunit);
    QCOMPARE(attr.expireToFolderId(), akonadiid);
}

void ExpireCollectionAttributeTest::shouldCloneAttr_data()
{
    QTest::addColumn<bool>("autoexpire");
    QTest::addColumn<int>("unreadexpireage");
    QTest::addColumn<int>("readexpireage");
    QTest::addColumn<MailCommon::ExpireCollectionAttribute::ExpireUnits>("unreadexpireunit");
    QTest::addColumn<MailCommon::ExpireCollectionAttribute::ExpireUnits>("readexpireunit");
    QTest::addColumn<MailCommon::ExpireCollectionAttribute::ExpireAction>("expireaction");
    QTest::addColumn<Akonadi::Collection::Id>("akonadiid");

    QTest::newRow("add autoexpire") << true << 28 << 14 << MailCommon::ExpireCollectionAttribute::ExpireNever
                                    << MailCommon::ExpireCollectionAttribute::ExpireNever << MailCommon::ExpireCollectionAttribute::ExpireDelete << qint64(5);
    QTest::newRow("change action") << true << 28 << 14 << MailCommon::ExpireCollectionAttribute::ExpireNever
                                   << MailCommon::ExpireCollectionAttribute::ExpireNever << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(5);
    QTest::newRow("change age") << true << 150 << 1 << MailCommon::ExpireCollectionAttribute::ExpireNever << MailCommon::ExpireCollectionAttribute::ExpireNever
                                << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(5);
    QTest::newRow("change units") << true << 15 << 1 << MailCommon::ExpireCollectionAttribute::ExpireDays << MailCommon::ExpireCollectionAttribute::ExpireDays
                                  << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(5);
    QTest::newRow("change units2") << true << 150 << 18 << MailCommon::ExpireCollectionAttribute::ExpireMonths
                                   << MailCommon::ExpireCollectionAttribute::ExpireWeeks << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(6);
}

void ExpireCollectionAttributeTest::shouldCloneAttr()
{
    QFETCH(bool, autoexpire);
    QFETCH(int, unreadexpireage);
    QFETCH(int, readexpireage);
    QFETCH(MailCommon::ExpireCollectionAttribute::ExpireUnits, unreadexpireunit);
    QFETCH(MailCommon::ExpireCollectionAttribute::ExpireUnits, readexpireunit);
    QFETCH(MailCommon::ExpireCollectionAttribute::ExpireAction, expireaction);
    QFETCH(Akonadi::Collection::Id, akonadiid);
    MailCommon::ExpireCollectionAttribute attr;
    attr.setAutoExpire(autoexpire);
    attr.setUnreadExpireAge(unreadexpireage);
    attr.setReadExpireAge(readexpireage);
    attr.setUnreadExpireUnits(unreadexpireunit);
    attr.setReadExpireUnits(readexpireunit);
    attr.setExpireAction(expireaction);
    attr.setExpireToFolderId(akonadiid);
    MailCommon::ExpireCollectionAttribute *result = attr.clone();
    QVERIFY(attr == *result);
    delete result;
}

void ExpireCollectionAttributeTest::shouldSerializedValue_data()
{
    QTest::addColumn<bool>("autoexpire");
    QTest::addColumn<int>("unreadexpireage");
    QTest::addColumn<int>("readexpireage");
    QTest::addColumn<MailCommon::ExpireCollectionAttribute::ExpireUnits>("unreadexpireunit");
    QTest::addColumn<MailCommon::ExpireCollectionAttribute::ExpireUnits>("readexpireunit");
    QTest::addColumn<MailCommon::ExpireCollectionAttribute::ExpireAction>("expireaction");
    QTest::addColumn<Akonadi::Collection::Id>("akonadiid");

    QTest::newRow("add autoexpire") << true << 28 << 14 << MailCommon::ExpireCollectionAttribute::ExpireNever
                                    << MailCommon::ExpireCollectionAttribute::ExpireNever << MailCommon::ExpireCollectionAttribute::ExpireDelete << qint64(5);
    QTest::newRow("change action") << true << 28 << 14 << MailCommon::ExpireCollectionAttribute::ExpireNever
                                   << MailCommon::ExpireCollectionAttribute::ExpireNever << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(5);
    QTest::newRow("change age") << true << 150 << 1 << MailCommon::ExpireCollectionAttribute::ExpireNever << MailCommon::ExpireCollectionAttribute::ExpireNever
                                << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(5);
    QTest::newRow("change units") << true << 15 << 1 << MailCommon::ExpireCollectionAttribute::ExpireDays << MailCommon::ExpireCollectionAttribute::ExpireDays
                                  << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(5);
    QTest::newRow("change units2") << true << 150 << 18 << MailCommon::ExpireCollectionAttribute::ExpireMonths
                                   << MailCommon::ExpireCollectionAttribute::ExpireWeeks << MailCommon::ExpireCollectionAttribute::ExpireMove << qint64(6);
}

void ExpireCollectionAttributeTest::shouldSerializedValue()
{
    QFETCH(bool, autoexpire);
    QFETCH(int, unreadexpireage);
    QFETCH(int, readexpireage);
    QFETCH(MailCommon::ExpireCollectionAttribute::ExpireUnits, unreadexpireunit);
    QFETCH(MailCommon::ExpireCollectionAttribute::ExpireUnits, readexpireunit);
    QFETCH(MailCommon::ExpireCollectionAttribute::ExpireAction, expireaction);
    QFETCH(Akonadi::Collection::Id, akonadiid);
    MailCommon::ExpireCollectionAttribute attr;
    attr.setAutoExpire(autoexpire);
    attr.setUnreadExpireAge(unreadexpireage);
    attr.setReadExpireAge(readexpireage);
    attr.setUnreadExpireUnits(unreadexpireunit);
    attr.setReadExpireUnits(readexpireunit);
    attr.setExpireAction(expireaction);
    attr.setExpireToFolderId(akonadiid);
    const QByteArray ba = attr.serialized();
    MailCommon::ExpireCollectionAttribute result;
    result.deserialize(ba);
    QVERIFY(attr == result);
}

void ExpireCollectionAttributeTest::shouldHaveType()
{
    MailCommon::ExpireCollectionAttribute attr;
    QCOMPARE(attr.type(), QByteArray("expirationcollectionattribute"));
}

QTEST_MAIN(ExpireCollectionAttributeTest)

#include "moc_expirecollectionattributetest.cpp"
