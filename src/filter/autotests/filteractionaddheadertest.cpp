/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filteractionaddheadertest.h"
#include "../filteractions/filteractionaddheader.h"
#include "filter/itemcontext.h"

#include <KLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTest>

FilterActionAddHeaderTest::FilterActionAddHeaderTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionAddHeaderTest::~FilterActionAddHeaderTest()
{
}

void FilterActionAddHeaderTest::shouldCreateWidget()
{
    MailCommon::FilterActionAddHeader filter;
    QWidget *widget = filter.createParamWidget(nullptr);
    QVERIFY(widget);
    auto comboBox = widget->findChild<QComboBox *>(QStringLiteral("combo"));
    QVERIFY(comboBox);
    QVERIFY(comboBox->isEditable());
    auto *label = widget->findChild<QLabel *>(QStringLiteral("label_value"));
    QVERIFY(label);
    auto *lineEdit = widget->findChild<KLineEdit *>(QStringLiteral("ledit"));
    QVERIFY(lineEdit);
    QVERIFY(lineEdit->text().isEmpty());
}

void FilterActionAddHeaderTest::shouldAddValue_data()
{
    QTest::addColumn<QString>("argsinput");
    QTest::addColumn<QString>("resultheader");
    QTest::addColumn<QString>("resultvalue");
    QTest::newRow("empty") << QString() << QString() << QString();
    QString val = QLatin1String("bla") + QLatin1Char('\t') + QLatin1String("blo");
    QTest::newRow("real value") << val << QStringLiteral("bla") << QStringLiteral("blo");
}

void FilterActionAddHeaderTest::shouldClearWidget()
{
    MailCommon::FilterActionAddHeader filter;
    QWidget *widget = filter.createParamWidget(nullptr);
    auto comboBox = widget->findChild<QComboBox *>(QStringLiteral("combo"));
    auto *lineEdit = widget->findChild<KLineEdit *>(QStringLiteral("ledit"));
    comboBox->lineEdit()->setText(QStringLiteral("test"));
    lineEdit->setText(QStringLiteral("blo"));
    filter.clearParamWidget(widget);
    QVERIFY(comboBox->lineEdit()->text().isEmpty());
    QVERIFY(lineEdit->text().isEmpty());
}

void FilterActionAddHeaderTest::shouldReturnSieveCode()
{
    MailCommon::FilterActionAddHeader filter;
    QCOMPARE(filter.sieveRequires().join(QLatin1Char(',')), QStringLiteral("editheader"));
}

void FilterActionAddHeaderTest::shouldBeEmpty()
{
    MailCommon::FilterActionAddHeader filter;
    QVERIFY(filter.isEmpty());

    filter.argsFromString(QString());
    QVERIFY(filter.isEmpty());

    filter.argsFromString(QStringLiteral("foo\tbla"));
    QVERIFY(!filter.isEmpty());
}

void FilterActionAddHeaderTest::shouldNotExecuteActionWhenParameterIsEmpty()
{
    MailCommon::FilterActionAddHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QLatin1String(""));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::ErrorButGoOn);
    QCOMPARE(context.needsPayloadStore(), false);
    QCOMPARE(context.deleteItem(), false);
    QCOMPARE(context.needsFlagStore(), false);
}

void FilterActionAddHeaderTest::shouldNotExecuteActionWhenValueIsEmpty()
{
    MailCommon::FilterActionAddHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QStringLiteral("foo"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::ErrorButGoOn);
    QCOMPARE(context.needsPayloadStore(), false);
    QCOMPARE(context.deleteItem(), false);
    QCOMPARE(context.needsFlagStore(), false);
}

void FilterActionAddHeaderTest::shouldAddNewHeaderWhenNotExistingHeader()
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
    const QByteArray output =
        "From: foo@kde.org\n"
        "To: foo@kde.org\n"
        "Subject: test\n"
        "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
        "MIME-Version: 1.0\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "testheader: foo\n"
        "\n"
        "test";

    MailCommon::FilterActionAddHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QStringLiteral("testheader\tfoo"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), true);
    QCOMPARE(msgPtr->encodedContent(), output);
    QCOMPARE(context.deleteItem(), false);
    QCOMPARE(context.needsFlagStore(), false);
}

void FilterActionAddHeaderTest::shouldReplaceHeaderWhenExistingHeader()
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
    const QByteArray output =
        "From: foo@kde.org\n"
        "To: foo@kde.org\n"
        "Subject: test\n"
        "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
        "MIME-Version: 1.0\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "testheader: foo\n"
        "\n"
        "test";

    MailCommon::FilterActionAddHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QStringLiteral("testheader\tfoo"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), true);
    QCOMPARE(msgPtr->encodedContent(), output);
    QCOMPARE(context.deleteItem(), false);
    QCOMPARE(context.needsFlagStore(), false);
}

void FilterActionAddHeaderTest::shouldAddValue()
{
    QFETCH(QString, argsinput);
    QFETCH(QString, resultheader);
    QFETCH(QString, resultvalue);

    MailCommon::FilterActionAddHeader filter;
    QWidget *widget = filter.createParamWidget(nullptr);
    filter.argsFromString(argsinput);
    filter.setParamWidgetValue(widget);
    auto comboBox = widget->findChild<QComboBox *>(QStringLiteral("combo"));
    auto *lineEdit = widget->findChild<KLineEdit *>(QStringLiteral("ledit"));
    QCOMPARE(comboBox->lineEdit()->text(), resultheader);
    QCOMPARE(lineEdit->text(), resultvalue);
}

void FilterActionAddHeaderTest::shouldRequiresSieve()
{
    MailCommon::FilterActionAddHeader filter;
    QCOMPARE(filter.sieveRequires(), QStringList() << QStringLiteral("editheader"));
}

QTEST_MAIN(FilterActionAddHeaderTest)
