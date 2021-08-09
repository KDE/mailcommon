/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionrewriteheadertest.h"
#include "../filteractions/filteractionrewriteheader.h"
#include <KLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTest>
#include <QWidget>

FilterActionRewriteHeaderTest::FilterActionRewriteHeaderTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionRewriteHeaderTest::~FilterActionRewriteHeaderTest()
{
}

void FilterActionRewriteHeaderTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionRewriteHeader filter;
    QWidget *w = filter.createParamWidget(nullptr);
    auto combo = w->findChild<QComboBox *>(QStringLiteral("combo"));
    QVERIFY(combo);
    QVERIFY(combo->count() > 0);

    auto *label = w->findChild<QLabel *>(QStringLiteral("label_replace"));
    QVERIFY(label);

    auto *regExpLineEdit = w->findChild<KLineEdit *>(QStringLiteral("search"));
    QVERIFY(regExpLineEdit);
    QVERIFY(regExpLineEdit->text().isEmpty());

    label = w->findChild<QLabel *>(QStringLiteral("label_with"));
    QVERIFY(label);

    auto *lineEdit = w->findChild<KLineEdit *>(QStringLiteral("replace"));
    QVERIFY(lineEdit);
    QVERIFY(lineEdit->text().isEmpty());
}

void FilterActionRewriteHeaderTest::shouldBeEmpty()
{
    MailCommon::FilterActionRewriteHeader filter;
    QVERIFY(filter.isEmpty());
    filter.argsFromString(QStringLiteral("foo\tbla"));
    QVERIFY(filter.isEmpty());
    filter.argsFromString(QStringLiteral("foo\tbla\tkde"));
    QVERIFY(!filter.isEmpty());
}

void FilterActionRewriteHeaderTest::shouldNotExecuteActionWhenParameterIsEmpty()
{
    MailCommon::FilterActionRewriteHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QString());
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::ErrorButGoOn);
    QCOMPARE(context.needsPayloadStore(), false);
}

void FilterActionRewriteHeaderTest::shouldNotExecuteActionWhenValueIsEmpty()
{
    MailCommon::FilterActionRewriteHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QStringLiteral("foo"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::ErrorButGoOn);
    QCOMPARE(context.needsPayloadStore(), false);

    filter.argsFromString(QStringLiteral("foo\tbla"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::ErrorButGoOn);
    QCOMPARE(context.needsPayloadStore(), false);
}

void FilterActionRewriteHeaderTest::shouldRewriteHeader()
{
    const QByteArray data =
        "From: foo@kde.org\n"
        "To: foo@kde.org\n"
        "Subject: test\n"
        "testheader: foo\n"
        "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
        "MIME-Version: 1.0\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "\n"
        "test";
    const QByteArray output =
        "From: foo@kde.org\n"
        "To: foo@kde.org\n"
        "Subject: test\n"
        "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
        "MIME-Version: 1.0\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "testheader: bla\n"
        "\n"
        "test";

    MailCommon::FilterActionRewriteHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QStringLiteral("testheader\tfoo\tbla"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), true);
    QCOMPARE(msgPtr->encodedContent(), output);
}

void FilterActionRewriteHeaderTest::shouldNotRewriteHeaderWhenHeaderNotFound()
{
    const QByteArray data =
        "From: foo@kde.org\n"
        "To: foo@kde.org\n"
        "Subject: test\n"
        "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
        "MIME-Version: 1.0\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "\n"
        "test";

    MailCommon::FilterActionRewriteHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QStringLiteral("testheader\tfoo\tbla"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), false);
    QCOMPARE(msgPtr->encodedContent(), data);
}

void FilterActionRewriteHeaderTest::shouldNotRewriteHeaderWhenRegexpNotFound()
{
    const QByteArray data =
        "From: foo@kde.org\n"
        "To: foo@kde.org\n"
        "Subject: test\n"
        "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
        "MIME-Version: 1.0\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "testheader: bla\n"
        "\n"
        "test";

    MailCommon::FilterActionRewriteHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QStringLiteral("testheader\tfoo\tbla"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), false);
    QCOMPARE(msgPtr->encodedContent(), data);
}

void FilterActionRewriteHeaderTest::shouldHaveRequiredPart()
{
    MailCommon::FilterActionRewriteHeader filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

QTEST_MAIN(FilterActionRewriteHeaderTest)
