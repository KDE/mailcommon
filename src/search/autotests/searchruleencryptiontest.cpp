/*
  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "searchruleencryptiontest.h"
#include "searchrule/searchruleencryption.h"

#include <QTest>

#include <AkonadiCore/Item>
#include <KMime/Message>

Q_DECLARE_METATYPE(MailCommon::SearchRule::Function)

using namespace MailCommon;

void SearchRuleEncryptionTest::shouldNotBeEmpty()
{
    SearchRuleEncryption rule;
    QCOMPARE(rule.isEmpty(), false);
}

void SearchRuleEncryptionTest::shouldRequiresPart()
{
    SearchRuleEncryption rule;
    QCOMPARE(rule.requiredPart(), SearchRule::CompleteMessage);
}

void SearchRuleEncryptionTest::shouldMatchMessage_data()
{
    QTest::addColumn<QByteArray>("message");
    QTest::addColumn<SearchRule::Function>("function");
    QTest::addColumn<bool>("shouldMatch");

    QByteArray msg;

    msg =
        "From: Sender <sender@example.com>\n"
        "To: Recipient <recipient@example.com>\n"
        "Subject: Test Message\n"
        "Date: Mon, 18 Sep 2017 16:33:57 +0200\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "\n"
        "So long and thanks for all the fish.\n";
    QTest::newRow("text/plain equals") << msg << SearchRule::FuncEquals << false;
    QTest::newRow("text/plain notequals") << msg << SearchRule::FuncNotEqual << true;

    msg =
        "From: Sender <sender@example.com>\n"
        "To: Recipient <recipient@example.com>\n"
        "Subject: Test Message\n"
        "Date: Mon, 18 Sep 2017 16:36:55 +0200\n"
        "Content-Type: multipart/alternative; boundary=\"nextPart4248936.HWsL1qCvO2\"\n"
        "\n"
        "This is a multi-part message in MIME format.\n"
        "\n"
        "--nextPart4248936.HWsL1qCvO2\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "\n"
        "/So long and thanks for all the fish/\n"
        "\n"
        "--nextPart4248936.HWsL1qCvO2\n"
        "Content-Type: text/html; charset=\"us-ascii\"\n"
        "\n"
        "<!DOCTYPE HTML>\n"
        "<html><head><p>So long and thanks for all the fish</p></body></html>\n"
        "--nextPart4248936.HWsL1qCvO2--\n";
    QTest::newRow("multipart equals") << msg << SearchRule::FuncEquals << false;
    QTest::newRow("multipart notequals") << msg << SearchRule::FuncNotEqual << true;

    msg =
        "From: Sender <sender@example.com>\n"
        "To: Recipient <recipient@example.com>\n"
        "Subject: Test Message\n"
        "Date: Mon, 18 Sep 2017 16:40:40 +0200\n"
        "Content-Type: multipart/encrypted; boundary=\"nextPart1821584.7Og3uilSh7\"; protocol=\"application/pgp-encrypted\"\n"
        "\n"
        "--nextPart1821584.7Og3uilSh7\n"
        "Content-Type: application/pgp-encrypted\n"
        "Content-Disposition: attachment\n"
        "\n"
        "Version: 1\n"
        "--nextPart1821584.7Og3uilSh7\n"
        "Content-Type: application/octet-stream\n"
        "Content-Disposition: inline; filename=\"msg.asc\"\n"
        "Content-Transfer-Encoding: 7Bit\n"
        "\n"
        "-----BEGIN PGP MESSAGE-----\n"
        "\n"
        "hQEMA923xmsQX9HEAQf9HoQNoqpAcOjPyR2ggO05o33FX3VQueGK6lTgvBYeyshK\n"
        "fLOaWvjtQ2ftoH3nd4mBBrKwaYHTrfPL/9ZgDJ8j8YXJtD68wOW150iXyCUBQasO\n"
        "9nQ59wd61JPECO6MoHItzJgpkeE+dujK5ULQJYiFinVn5HZEqEizOEqTlFJUN86d\n"
        " we will not look into the body, so we don't need the full body \n"
        "hJKWywAfRqRJBq/fQmoXMQ+IvsBQx7KZ92eO2NvkGimEzHX3Uiwi6kuORFiLrFqJ\n"
        "Uc0mhna4cBU4bt3Zg2i0jQ4AJUZiXKg=\n"
        "=96px\n"
        "-----END PGP MESSAGE-----\n"
        "\n"
        "--nextPart1821584.7Og3uilSh7--\n";
    QTest::newRow("gpg equals") << msg << SearchRule::FuncEquals << true;
    QTest::newRow("gpg notequals") << msg << SearchRule::FuncNotEqual << false;

    msg =
        "From: Sender <sender@example.com>\n"
        "To: Recipient <recipient@example.com>\n"
        "Subject: Test Message\n"
        "Date: Mon, 18 Sep 2017 16:40:40 +0200\n"
        "Content-Type: text/plain; charset=\"us-ascii\"\n"
        "\n"
        "-----BEGIN PGP MESSAGE-----\n"
        "\n"
        "hQEMA923xmsQX9HEAQf/Zu+L2EqkoAhacPlns466Chw4Re5q+zPQgVYhmXW4KkQ3\n"
        "bVzkXJ0ZiWamfimc/JBjU3gfXcCBPgnwdPJXxS9U8mIrgzLRIc4xGLL4OKidZibC\n"
        "HzmfC1ScJjWz2UE4KkVRSfEeCzVn+7S3V4DrPYjN3XGR4hGpV+gDDfqrw563KqVZ\n"
        " we will not look into the body, so we don't need the full body \n"
        "URsJzzMVfl2CacwT2GWuthv6g0idzNardrircmefgT2ACMvxGSbTmfw9T5McuiRu\n"
        "6nJFyi9rE948y8B5IjdXazaOyf8=\n"
        "=KRPN\n"
        "-----END PGP MESSAGE-----\n";
    QTest::newRow("gpg-inline equals") << msg << SearchRule::FuncEquals << true;
    QTest::newRow("gpg-inline notequals") << msg << SearchRule::FuncNotEqual << false;

    msg =
        "From: Sender <sender@example.com>\n"
        "To: Recipient <recipient@example.com>\n"
        "Subject: Test message\n"
        "Date: Mon, 18 Sep 2017 16:50:22 +0200\n"
        "Content-Type: application/pkcs7-mime; name=\"smime.p7m\"; smime-type=\"enveloped-data\"\n"
        "Content-Transfer-Encoding: base64\n"
        "Content-Disposition: attachment; filename=\"smime.p7m\"\n"
        "\n"
        "MIAGCSqGSIb3DQEHA6CAMIACAQAxggHNMIIByQIBADCBsDCBmzELMAkGA1UEBhMCR0IxGzAZBgNV\n"
        "BAgTEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UEBxMHU2FsZm9yZDEaMBgGA1UEChMRQ09NT0RP\n"
        "a57rVj7FnjHhlBgpsYGpv8cDu2f60JLHWcKJwaJHnqVABWVfE7Dp5LHP1U96tdJjiZ1EsPiWalT4\n"
        "Tto8U1MEsyZGJzm0Xfs8oKyZx63mp0WfRLXdfo+PFLF0MaFcd4ITo/KstX2VY8FNtVjqAZ/aE5+f\n"
        "...... we will not look into the body, so we don't need the full body ......\n"
        "ulxttPf2lx34UjZPPTmhBJT91tPlbN3oiezl5dYwgAYJKoZIhvcNAQcBMB0GCWCGSAFlAwQBAgQQ\n"
        "4CBkVjsRMzvAVfnOFJm9KP0ex3pK/t1zotsVUaGXm28OTIL6xRJp1TLdhy6HFIiMGmyKPSMDq//F\n"
        "sonU51JpPcqs1G/XFUTgrPZ4R5zBQyclGwQsY3YQR+ik2iBkk4rjiJbI1chVp9kLEY1YxfABY/vo\n"
        "LVURWHOzXAlxzbVz8hY0TJ9Nr2wyrkUR52UhBBBvxNjI3yM2XVxUQflmayVkAAAAAAAAAAAAAA==\n";
    QTest::newRow("smime equals") << msg << SearchRule::FuncEquals << true;
    QTest::newRow("smime notequals") << msg << SearchRule::FuncNotEqual << false;
}

void SearchRuleEncryptionTest::shouldMatchMessage()
{
    QFETCH(QByteArray, message);
    QFETCH(SearchRule::Function, function);
    QFETCH(bool, shouldMatch);

    auto msg = KMime::Message::Ptr::create();
    msg->setContent(message);
    msg->parse();

    Akonadi::Item item(KMime::Message::mimeType());
    item.setPayload(msg);

    SearchRuleEncryption rule({}, function, {});
    QCOMPARE(rule.matches(item), shouldMatch);
}

QTEST_MAIN(SearchRuleEncryptionTest)
