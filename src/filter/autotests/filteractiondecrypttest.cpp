/*
 *  Copyright (c) 2017  Daniel Vrátil <dvratil@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License, version 2, as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "filteractiondecrypttest.h"
#include "../filteractions/filteractiondecrypt.h"

#include <QTest>

void FilterActionDecryptTest::initTestCase()
{
    mGpg = new GPGHelper(QString::fromUtf8(TEST_PATH) + QStringLiteral("/gpghome"));
    QVERIFY(mGpg->isValid());
}

void FilterActionDecryptTest::cleanupTestCase()
{
    delete mGpg;
}

void FilterActionDecryptTest::shouldDecrypt_data()
{
    QTest::addColumn<QByteArray>("content");
    QTest::addColumn<QByteArray>("encrypted");

    QDir testDir(QString::fromUtf8(TEST_PATH) + QStringLiteral("/gpgdata"));
    const auto tests = testDir.entryInfoList({ QStringLiteral("*.msg") }, QDir::Files, QDir::Name);
    for (const auto test : tests) {
        QFile plain(test.absoluteFilePath());
        QVERIFY(plain.open(QIODevice::ReadOnly));
        const auto plainData = plain.readAll();

        QFile pgp(test.absoluteFilePath() + QStringLiteral(".pgp"));
        QVERIFY(pgp.open(QIODevice::ReadOnly));
        QTest::newRow(QStringLiteral("PGP %1").arg(test.baseName()).toUtf8().constData())
            << plainData << pgp.readAll();

        QFile smime(test.absoluteFilePath() + QStringLiteral(".smime"));
        QVERIFY(smime.open(QIODevice::ReadOnly));
        QTest::newRow(QStringLiteral("SMIME %1").arg(test.baseName()).toUtf8().constData())
            << plainData << smime.readAll();

        QTest::newRow(QStringLiteral("PLAIN %1").arg(test.baseName()).toUtf8().constData())
            << plainData << plainData;
    }

}

void FilterActionDecryptTest::shouldDecrypt()
{
    QFETCH(QByteArray, content);
    QFETCH(QByteArray, encrypted);

    MailCommon::FilterActionDecrypt action(this);

    auto msg = KMime::Message::Ptr::create();
    msg->setContent(encrypted);
    msg->parse();
    msg->assemble();

    Akonadi::Item item;
    item.setPayload(msg);

    MailCommon::ItemContext context(item, true);
    const auto result = action.process(context, false);
    QCOMPARE(result, MailCommon::FilterAction::GoOn);
    if (content != encrypted) {
        QVERIFY(context.needsPayloadStore());
    } else {
        // the message is not encrypted, no change is needed
        QVERIFY(!context.needsPayloadStore());
    }

    auto newMsg = context.item().payload<KMime::Message::Ptr>();
    QCOMPARE(newMsg->from()->asUnicodeString(), msg->from()->asUnicodeString());
    QCOMPARE(newMsg->to()->asUnicodeString(), msg->to()->asUnicodeString());
    QCOMPARE(newMsg->date()->asUnicodeString(), msg->date()->asUnicodeString());
    QCOMPARE(newMsg->subject()->asUnicodeString(), msg->subject()->asUnicodeString());

    auto decrypted = newMsg->encodedContent();
    KMime::Message decryptedContent;
    decryptedContent.setContent(newMsg->encodedContent());
    decryptedContent.parse();
    KMime::Message expectedContent;
    expectedContent.setContent(content);
    expectedContent.parse();
    QCOMPARE(decryptedContent.from()->asUnicodeString(), expectedContent.from()->asUnicodeString());
    QCOMPARE(decryptedContent.to()->asUnicodeString(), expectedContent.to()->asUnicodeString());
    QCOMPARE(decryptedContent.date()->asUnicodeString(), expectedContent.date()->asUnicodeString());
    QCOMPARE(decryptedContent.subject()->asUnicodeString(), expectedContent.subject()->asUnicodeString());
    QCOMPARE(decryptedContent.contentType()->asUnicodeString(), expectedContent.contentType()->asUnicodeString());
    QCOMPARE(decryptedContent.encodedBody(), expectedContent.encodedBody());
}

QTEST_MAIN(FilterActionDecryptTest)
