/*
 *  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only
 */

#include "filteractionencrypttest.h"
#include "../filteractions/filteractionencrypt.h"

#include <QTest>

void FilterActionEncryptTest::initTestCase()
{
    mGpg = new GPGHelper(QStringLiteral(TEST_PATH) + QStringLiteral("/gpghome"));
    QVERIFY(mGpg->isValid());
}

void FilterActionEncryptTest::cleanupTestCase()
{
    delete mGpg;
}

void FilterActionEncryptTest::shouldEncrypt_data()
{
    QTest::addColumn<QString>("key");
    QTest::addColumn<QByteArray>("content"); // content for decryption
    QTest::addColumn<QByteArray>("expected"); // decrypted expected content

    const auto smimeKey = QStringLiteral("SMIME:%1:0FDD972BCEFB5735DC7E8EE57DB7BA4E5FDBE218");
    const auto pgpKey = QStringLiteral("PGP:%1:818AE8DA30F81B0CEA4403BA358732559B8659B2");

    QDir testDir(QStringLiteral(TEST_PATH) + QStringLiteral("/gpgdata"));
    const auto tests = testDir.entryInfoList({QStringLiteral("*.msg")}, QDir::Files, QDir::Name);
    for (const auto &test : tests) {
        QFile plain(test.absoluteFilePath());
        QVERIFY(plain.open(QIODevice::ReadOnly));
        const auto plainData = plain.readAll();

        QTest::newRow(QStringLiteral("PGP %1").arg(test.baseName()).toUtf8().constData()) << pgpKey.arg(0) << plainData << plainData;
        QTest::newRow(QStringLiteral("SMIME %1").arg(test.baseName()).toUtf8().constData()) << smimeKey.arg(0) << plainData << plainData;

        QFile smimeFile(test.absoluteFilePath() + QStringLiteral(".smime"));
        QVERIFY(smimeFile.open(QIODevice::ReadOnly));
        const auto smimeData = smimeFile.readAll();
        QFile pgpFile(test.absoluteFilePath() + QStringLiteral(".pgp"));
        QVERIFY(pgpFile.open(QIODevice::ReadOnly));
        const auto pgpData = pgpFile.readAll();

        QTest::newRow(QStringLiteral("PGP %1 re-encrypt").arg(test.baseName()).toUtf8().constData()) << pgpKey.arg(1) << smimeData << plainData;
        QTest::newRow(QStringLiteral("SMIME %1 re-encrypt").arg(test.baseName()).toUtf8().constData()) << smimeKey.arg(1) << pgpData << plainData;
        QTest::newRow(QStringLiteral("PGP %1 re-encrypt same key").arg(test.baseName()).toUtf8().constData()) << pgpKey.arg(1) << pgpData << plainData;
    }
}

void FilterActionEncryptTest::shouldEncrypt()
{
    QFETCH(QString, key);
    QFETCH(QByteArray, content);
    QFETCH(QByteArray, expected);

    MailCommon::FilterActionEncrypt action(this);
    action.argsFromString(key);
    QVERIFY(!action.key().isNull());
    QCOMPARE(action.reencrypt(), key.contains(QLatin1String(":1:")));

    auto msg = KMime::Message::Ptr::create();
    msg->setContent(content);
    msg->parse();
    msg->assemble();

    Akonadi::Item item;
    item.setPayload(msg);

    MailCommon::ItemContext context(item, true);
    const auto result = action.process(context, false);
    QCOMPARE(result, MailCommon::FilterAction::GoOn);
    QVERIFY(context.needsPayloadStore());

    auto newMsg = context.item().payload<KMime::Message::Ptr>();
    QCOMPARE(newMsg->from()->asUnicodeString(), msg->from()->asUnicodeString());
    QCOMPARE(newMsg->to()->asUnicodeString(), msg->to()->asUnicodeString());
    QCOMPARE(newMsg->date()->asUnicodeString(), msg->date()->asUnicodeString());
    QCOMPARE(newMsg->subject()->asUnicodeString(), msg->subject()->asUnicodeString());

    QByteArray resultContent;
    GPGHelper::CryptoType crypto;
    if (key.startsWith(QLatin1String("PGP"))) {
        QCOMPARE(newMsg->contentType()->mimeType(), QByteArray("multipart/encrypted"));
        resultContent = newMsg->encodedContent();
        crypto = GPGHelper::OpenPGP;
    } else {
        QCOMPARE(newMsg->contentType()->mimeType(), QByteArray("application/pkcs7-mime"));
        resultContent = QByteArray::fromBase64(newMsg->encodedBody());
        crypto = GPGHelper::SMIME;
    }

    // Check if the message is encrypted with the right key
    const auto usedKey = mGpg->encryptionKeyFp(resultContent, crypto);
    QCOMPARE(usedKey, QString::fromLatin1(action.key().primaryFingerprint()));

    const auto actual = mGpg->decrypt(resultContent, crypto);

    KMime::Message actualContent;
    actualContent.setContent(actual);
    actualContent.parse();
    KMime::Message expectedContent;
    expectedContent.setContent(expected);
    expectedContent.parse();
    QCOMPARE(actualContent.from()->asUnicodeString(), expectedContent.from()->asUnicodeString());
    QCOMPARE(actualContent.to()->asUnicodeString(), expectedContent.to()->asUnicodeString());
    QCOMPARE(actualContent.date()->asUnicodeString(), expectedContent.date()->asUnicodeString());
    QCOMPARE(actualContent.subject()->asUnicodeString(), expectedContent.subject()->asUnicodeString());
    QCOMPARE(actualContent.contentType()->asUnicodeString(), expectedContent.contentType()->asUnicodeString());
    QCOMPARE(actualContent.encodedBody(), expectedContent.encodedBody());
}

QTEST_MAIN(FilterActionEncryptTest)
