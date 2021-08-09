/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionreplytotest.h"
#include "../filteractions/filteractionreplyto.h"
#include <QTest>
#include <QWidget>
FilterActionReplyToTest::FilterActionReplyToTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionReplyToTest::~FilterActionReplyToTest()
{
}

void FilterActionReplyToTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionReplyTo filter;
    QWidget *w = filter.createParamWidget(nullptr);
    QCOMPARE(w->objectName(), QStringLiteral("emailaddressrequester"));
}

void FilterActionReplyToTest::shouldBeEmpty()
{
    MailCommon::FilterActionReplyTo filter;
    QVERIFY(filter.isEmpty());
}

void FilterActionReplyToTest::shouldHadReplyToHeader()
{
    const QString replyTo = QStringLiteral("fooreply@kde.org");

    const QByteArray data =
        "From: foo@kde.org\n"
        "To: foo@kde.org\n"
        "Subject: test\n"
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
        "Reply-To: fooreply@kde.org\n"
        "\n"
        "test";

    MailCommon::FilterActionReplyTo filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(replyTo);
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), true);
    QCOMPARE(msgPtr->encodedContent(), output);
}

void FilterActionReplyToTest::shouldReplaceReplyToHeader()
{
    const QString replyTo = QStringLiteral("fooreply@kde.org");

    const QByteArray data =
        "From: foo@kde.org\n"
        "To: foo@kde.org\n"
        "Subject: test\n"
        "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
        "MIME-Version: 1.0\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "Reply-To: oldfooreply@kde.org\n"
        "\n"
        "test";
    const QByteArray output =
        "From: foo@kde.org\n"
        "To: foo@kde.org\n"
        "Subject: test\n"
        "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
        "MIME-Version: 1.0\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "Reply-To: fooreply@kde.org\n"
        "\n"
        "test";

    MailCommon::FilterActionReplyTo filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(replyTo);
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), true);
    QCOMPARE(msgPtr->encodedContent(), output);
}

void FilterActionReplyToTest::shouldHaveRequiredPart()
{
    MailCommon::FilterActionReplyTo filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

void FilterActionReplyToTest::shouldNotCreateReplyToWhenAddressIsEmpty()
{
    const QByteArray data =
        "From: foo@kde.org\n"
        "To: foo@kde.org\n"
        "Subject: test\n"
        "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
        "MIME-Version: 1.0\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "Reply-To: oldfooreply@kde.org\n"
        "\n"
        "test";

    MailCommon::FilterActionReplyTo filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QString());
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::ErrorButGoOn);
    QCOMPARE(context.needsPayloadStore(), false);
    QCOMPARE(msgPtr->encodedContent(), data);
}

QTEST_MAIN(FilterActionReplyToTest)
