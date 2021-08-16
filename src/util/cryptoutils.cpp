/*
 * SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "cryptoutils.h"
#include "mailcommon_debug.h"

#include <QGpgME/DecryptJob>
#include <QGpgME/Protocol>
#include <QGpgME/VerifyOpaqueJob>

#include <gpgme++/context.h>
#include <gpgme++/decryptionresult.h>
#include <gpgme++/gpgmepp_version.h>
#include <gpgme++/verificationresult.h>

using namespace MailCommon;

bool CryptoUtils::isInlinePGP(const KMime::Content *part)
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

bool CryptoUtils::isPGP(const KMime::Content *part, bool allowOctetStream)
{
    const auto ct = static_cast<KMime::Headers::ContentType *>(part->headerByType("Content-Type"));
    return ct && (ct->isSubtype("pgp-encrypted") || ct->isSubtype("encrypted") || (allowOctetStream && ct->isMimeType("application/octet-stream")));
}

bool CryptoUtils::isSMIME(const KMime::Content *part)
{
    const auto ct = static_cast<KMime::Headers::ContentType *>(part->headerByType("Content-Type"));
    return ct && (ct->isSubtype("pkcs7-mime") || ct->isSubtype("x-pkcs7-mime"));
}

bool CryptoUtils::isEncrypted(const KMime::Message *msg)
{
    // KMime::isEncrypted does not cover all cases - mostly only deals with
    // mime types.
    if (KMime::isEncrypted(const_cast<KMime::Message *>(msg))) {
        return true;
    }

    return isInlinePGP(msg);
}

KMime::Message::Ptr CryptoUtils::decryptMessage(const KMime::Message::Ptr &msg, bool &wasEncrypted)
{
    GpgME::Protocol protoName = GpgME::UnknownProtocol;
    bool inlinePGP = false;
    bool multipart = false;
    if (msg->contentType(false) && msg->contentType(false)->isMimeType("multipart/encrypted")) {
        multipart = true;
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
    auto inData = multipart ? msg->encodedContent() : msg->decodedContent(); // decodedContent in fact returns decoded body
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
        decCt.setBody(KMime::CRLFtoLF(outData));
    } else {
        decCt.setContent(KMime::CRLFtoLF(outData));
    }
    decCt.parse();
    decCt.assemble();

    return assembleMessage(msg, &decCt);
}

void CryptoUtils::copyHeader(const KMime::Headers::Base *header, KMime::Message::Ptr msg)
{
    auto newHdr = KMime::Headers::createHeader(header->type());
    if (!newHdr) {
        newHdr = new KMime::Headers::Generic(header->type());
    }
    newHdr->from7BitString(header->as7BitString(false));
    msg->appendHeader(newHdr);
}

bool CryptoUtils::isContentHeader(const KMime::Headers::Base *header)
{
    return header->is("Content-Type") || header->is("Content-Transfer-Encoding") || header->is("Content-Disposition");
}

KMime::Message::Ptr CryptoUtils::assembleMessage(const KMime::Message::Ptr &orig, const KMime::Content *newContent)
{
    auto out = KMime::Message::Ptr::create();
    // Use the new content as message content
    out->setBody(const_cast<KMime::Content *>(newContent)->encodedBody());
    out->parse();

    // remove default explicit content headers added by KMime::Content::parse()
    QVector<KMime::Headers::Base *> headers = out->headers();
    for (const auto hdr : std::as_const(headers)) {
        if (isContentHeader(hdr)) {
            out->removeHeader(hdr->type());
        }
    }

    // Copy over headers from the original message, except for CT, CTE and CD
    // headers, we want to preserve those from the new content
    headers = orig->headers();
    for (const auto hdr : std::as_const(headers)) {
        if (isContentHeader(hdr)) {
            continue;
        }

        copyHeader(hdr, out);
    }

    // Overwrite some headers by those provided by the new content
    headers = newContent->headers();
    for (const auto hdr : std::as_const(headers)) {
        if (isContentHeader(hdr)) {
            copyHeader(hdr, out);
        }
    }

    out->assemble();
    out->parse();

    return out;
}
