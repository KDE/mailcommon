/*
 * Copyright (c) 2017  Daniel Vrátil <dvratil@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "filteractionwithcrypto.h"
#include "mailcommon_debug.h"

#include <QGpgME/Protocol>
#include <QGpgME/DecryptJob>
#include <QGpgME/VerifyOpaqueJob>

#include <gpgme++/decryptionresult.h>
#include <gpgme++/verificationresult.h>
#include <gpgme++/context.h>
#include <gpgme++/gpgmepp_version.h>

#include <QProcess>
#include <QStandardPaths>
#include <QRegularExpression>

using namespace MailCommon;

bool FilterActionWithCrypto::isInlinePGP(const KMime::Content *part) const
{
    // Find if the message body starts with --BEGIN PGP MESSAGE-- - we can't just
    // use contains(), because that would also qualify messages that mention the
    // string, but are not actually encrypted
    const auto body = part->body();
    for (auto c = body.cbegin(), end = body.cend(); c != end; ++c) {
        if (!c) { // huh?
            return false; // empty body -> not encrypted
        }
        // Is it a white space? Let's check next one
        if (isspace(*c)) {
            continue;
        }

        // First non-white space character in the body - if it's BEGIN PGP MESSAGE
        // then the message is encrypted, otherwise it's not.
        if (strncmp(c, "-----BEGIN PGP MESSAGE-----", sizeof("-----BEGIN PGP MESSAGE-----") - 1) == 0) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}

bool FilterActionWithCrypto::isPGP(const KMime::Content *part, bool allowOctetStream) const
{
    const auto ct = static_cast<KMime::Headers::ContentType*>(part->headerByType("Content-Type"));
    return ct && (ct->isSubtype("pgp-encrypted")
                    || ct->isSubtype("encrypted")
                    || (allowOctetStream && ct->isMimeType("application/octet-stream")));
}

bool FilterActionWithCrypto::isSMIME(const KMime::Content *part) const
{
    const auto ct = static_cast<KMime::Headers::ContentType*>(part->headerByType("Content-Type"));
    return ct && (ct->isSubtype("pkcs7-mime") || ct->isSubtype("x-pkcs7-mime"));
}

bool FilterActionWithCrypto::isEncrypted(const KMime::Message *msg) const
{
    // KMime::isEncrypted does not cover all cases - mostly only deals with
    // mime types.
    if (KMime::isEncrypted(const_cast<KMime::Message*>(msg))) {
        return true;
    }

    return isInlinePGP(msg);
}

KMime::Message::Ptr FilterActionWithCrypto::decryptMessage(const KMime::Message::Ptr &msg,
                                                           bool &wasEncrypted) const
{
    GpgME::Protocol protoName = GpgME::UnknownProtocol;
    bool inlinePGP = false;
    if (msg->mainBodyPart("multipart/encrypted")) {
        const auto subparts = msg->contents();
        for (auto subpart : subparts) {
            if (isPGP(subpart, true)) {
                protoName = GpgME::OpenPGP;
                break;
            } else if (isSMIME(subpart)) {
                protoName = GpgME::CMS;
                break;
            }
        }
    } else {
        if (isPGP(msg.data())) {
            protoName = GpgME::OpenPGP;
        } else if (isSMIME(msg.data())) {
            protoName = GpgME::CMS;
        } else if (isInlinePGP(msg.data())) {
            protoName = GpgME::OpenPGP;
            inlinePGP = true;
        }
    }

    if (protoName == GpgME::UnknownProtocol) {
        // Not encrypted, or we don't recognize the encryption
        wasEncrypted = false;
        return {};
    }

    const auto proto = (protoName == GpgME::OpenPGP) ? QGpgME::openpgp() : QGpgME::smime();

    wasEncrypted = true;
    QByteArray outData;
    auto inData = msg->decodedContent(); // decodedContent in fact returns decoded body

    auto decrypt = proto->decryptJob();
#if GPGMEPP_VERSION_MAJOR > 1 || (GPGMEPP_VERSION_MAJOR == 1 && GPGMEPP_VERSION_MINOR >= 9)
    if (inlinePGP) {
        auto ctx = QGpgME::Job::context(decrypt);
        ctx->setDecryptionFlags(GpgME::Context::DecryptUnwrap);
    }
#endif
    auto result = decrypt->exec(inData, outData);
    if (result.error()) {
        // unknown key, invalid algo, or general error
        qCWarning(MAILCOMMON_LOG) << "Failed to decrypt:" << result.error().asString();
        return {};
    }

    if (inlinePGP) {
        inData = outData;
        auto verify = proto->verifyOpaqueJob(true);
        auto result = verify->exec(inData, outData);
        if (result.error()) {
            qCWarning(MAILCOMMON_LOG) << "Failed to verify:" << result.error().asString();
            return {};
        }
    }


    KMime::Content decCt;
    if (inlinePGP) {
        decCt.setBody(outData);
    } else {
        decCt.setContent(outData);
    }
    decCt.parse();
    decCt.assemble();

    return assembleMessage(msg, &decCt);
}

void FilterActionWithCrypto::copyHeader(const KMime::Headers::Base *header,
                                        KMime::Message::Ptr msg) const
{
    auto newHdr = KMime::Headers::createHeader(header->type());
    if (!newHdr) {
        newHdr = new KMime::Headers::Generic(header->type());
    }
    newHdr->fromUnicodeString(header->asUnicodeString(), "UTF-8");
    msg->appendHeader(newHdr);
}

bool FilterActionWithCrypto::isContentHeader(const KMime::Headers::Base *header) const
{
    return header->is("Content-Type")
            || header->is("Content-Transfer-Encoding")
            || header->is("Content-Disposition");
}

KMime::Message::Ptr FilterActionWithCrypto::assembleMessage(const KMime::Message::Ptr &orig,
                                                            const KMime::Content *newContent) const
{
    auto out = KMime::Message::Ptr::create();
    // Use the new content as message content
    out->setBody(const_cast<KMime::Content*>(newContent)->encodedBody());
    out->parse();

    // Copy over headers from the original message, except for CT, CTE and CD
    // headers, we want to preserve those from the new content
    QVector<KMime::Headers::Base*> headers = orig->headers();
    for (const auto hdr : qAsConst(headers)) {
        if (isContentHeader(hdr)) {
            continue;
        }

        copyHeader(hdr, out);
    }

    // Overwrite some headers by those provided by the new content
    headers = newContent->headers();
    for (const auto hdr : qAsConst(headers)) {
        if (isContentHeader(hdr)) {
            copyHeader(hdr, out);
        }
    }

    out->assemble();
    out->parse();

    return out;
}

QStringList FilterActionWithCrypto::getEncryptionKeysFromContent(const KMime::Message::Ptr &msg,
                                                                 GpgME::Protocol protocol) const
{
    if (protocol == GpgME::CMS && mGpgSmPath.isNull()) {
        auto path = QStandardPaths::findExecutable(QStringLiteral("gpgsm"));
        mGpgSmPath = path.isEmpty() ? QStringLiteral("") : path;
    } else if (protocol == GpgME::OpenPGP && mGpgPath.isNull()) {
        auto path = QStandardPaths::findExecutable(QStringLiteral("gpg2"));
        if (path.isEmpty()) {
            path = QStandardPaths::findExecutable(QStringLiteral("gpg"));
            mGpgPath = path.isEmpty() ? QStringLiteral("") : path;
        } else {
            mGpgPath = path;
        }
    }

    if ((protocol == GpgME::CMS && mGpgSmPath.isEmpty())
        || (protocol == GpgME::OpenPGP && mGpgPath.isEmpty())) {
        return {};
    }

    QProcess gpg;
    QStringList keyIds;
    // TODO: contribute an API for this into gpgme
    if (protocol == GpgME::OpenPGP) {
        gpg.setProgram(mGpgPath);
        // --list-packets will give us list of keys used to encrypt the message
        // --batch will prevent gpg from asking for decryption password (we don't need it yet)
        gpg.setArguments({ QStringLiteral("--list-packets"), QStringLiteral("--batch") });
        gpg.start(QIODevice::ReadWrite);
        gpg.waitForStarted();
        gpg.write(msg->encodedContent());
        gpg.closeWriteChannel();
        gpg.waitForFinished();
        while (!gpg.atEnd()) {
            const auto l = gpg.readLine();
            if (l.startsWith(":pubkey")) {
                const int pos = l.indexOf("keyid ");
                if (pos < 0) {
                    continue;
                }
                const int start = pos + 6; // strlen("keyid ")
                const int len = l.size() - start - 1; // -1 to skip trailing \n
                keyIds << QString::fromUtf8(l.mid(start, len));
            }
        }
    } else if (protocol == GpgME::CMS) {
        gpg.setProgram(mGpgSmPath);
        // --decrypt - the only way how to get the keys from gpgsm, sadly, is to decrypt the email
        // --status-fd 2 - make sure the status output is not mangled with the decrypted content
        // --assume-base64 - so that we don't have to decode it ourselves
        gpg.setArguments({ QStringLiteral("--decrypt"),
                           QStringLiteral("--status-fd"), QStringLiteral("2"),
                           QStringLiteral("--debug-level"), QStringLiteral("basic"),
                           QStringLiteral("--assume-base64") });
        gpg.start(QIODevice::ReadWrite);
        gpg.waitForStarted();
        gpg.write(msg->encodedBody()); // just the body!
        gpg.closeWriteChannel();
        gpg.waitForFinished();
        gpg.setReadChannel(QProcess::StandardError);
        while (!gpg.atEnd()) {
            const auto l = gpg.readLine();
            if (l.startsWith("gpgsm: DBG: recp ")) {
                const int pos = l.indexOf("serial: ");
                if (pos < 0) {
                    continue;
                }
                const int start = pos + 8; // strlen("serial: ")
                const int len = l.size() - start - 1; // -1 to skip trailing \n
                keyIds << QString::fromUtf8(l.mid(start, len));
            }
        }
    }

    return keyIds;
}

