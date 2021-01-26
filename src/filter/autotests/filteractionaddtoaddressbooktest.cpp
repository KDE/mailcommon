/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionaddtoaddressbooktest.h"
#include "../filteractions/filteractionaddtoaddressbook.h"
#include <QComboBox>
#include <QLabel>
#include <QTest>
#include <QWidget>

#include <AkonadiWidgets/CollectionComboBox>
#include <AkonadiWidgets/TagWidget>

FilterActionAddToAddressBookTest::FilterActionAddToAddressBookTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionAddToAddressBookTest::~FilterActionAddToAddressBookTest()
{
}

void FilterActionAddToAddressBookTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionAddToAddressBook filter;
    QWidget *w = filter.createParamWidget(nullptr);

    auto headerCombo = w->findChild<QComboBox *>(QStringLiteral("HeaderComboBox"));
    QVERIFY(headerCombo);

    auto *label = w->findChild<QLabel *>(QStringLiteral("label_with_category"));
    QVERIFY(label);

    auto categoryEdit = w->findChild<Akonadi::TagWidget *>(QStringLiteral("CategoryEdit"));
    QVERIFY(categoryEdit);

    label = w->findChild<QLabel *>(QStringLiteral("label_in_addressbook"));
    QVERIFY(label);

    auto *collectionComboBox = w->findChild<Akonadi::CollectionComboBox *>(QStringLiteral("AddressBookComboBox"));
    QVERIFY(collectionComboBox);
}

void FilterActionAddToAddressBookTest::shouldReportErrorWhenArgumentIsEmpty()
{
    MailCommon::FilterActionAddToAddressBook filter;
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, false);

    filter.argsFromString(QString());
    QVERIFY(filter.isEmpty());
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::ErrorButGoOn);
    QCOMPARE(context.needsPayloadStore(), false);
    QCOMPARE(context.needsFlagStore(), false);
    QCOMPARE(context.needsFullPayload(), false);
}

void FilterActionAddToAddressBookTest::shouldReportErrorWhenCollectionIsInvalid()
{
    MailCommon::FilterActionAddToAddressBook filter;
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, false);

    filter.argsFromString(QStringLiteral("foo\t-1\tddd"));
    QVERIFY(filter.isEmpty());
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::ErrorButGoOn);
    QCOMPARE(context.needsPayloadStore(), false);
    QCOMPARE(context.needsFlagStore(), false);
    QCOMPARE(context.needsFullPayload(), false);
}

void FilterActionAddToAddressBookTest::shouldRequiresPart()
{
    MailCommon::FilterActionAddToAddressBook filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::Envelope);
}

QTEST_MAIN(FilterActionAddToAddressBookTest)
