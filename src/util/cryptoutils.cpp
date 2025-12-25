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
#include <gpgme++/verificationresult.h>
#include <gpgme.h>

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
    const auto ct = part->contentType();
    return ct && (ct->isSubtype("pgp-encrypted") || ct->isSubtype("encrypted") || (allowOctetStream && ct->isMimeType("application/octet-stream")));
}

bool CryptoUtils::isSMIME(const KMime::Content *part)
{
    const auto ct = part->contentType();
    return ct && (ct->isSubtype("pkcs7-mime") || ct->isSubtype("x-pkcs7-mime"));
}

bool CryptoUtils::isEncrypted(const KMime::Message *msg)
{
    // KMime::isEncrypted does not cover all cases - mostly only deals with
    // mime types.
    if (KMime::isEncrypted(msg)) {
        return true;
    }

    return isInlinePGP(msg);
}

std::shared_ptr<KMime::Message> CryptoUtils::decryptMessage(const std::shared_ptr<KMime::Message> &msg, bool &wasEncrypted)
{
    GpgME::Protocol protoName = GpgME::UnknownProtocol;
    bool inlinePGP = false;
    bool multipart = false;
    if (msg->contentType(KMime::CreatePolicy::DontCreate) && msg->contentType(KMime::CreatePolicy::DontCreate)->isMimeType("multipart/encrypted")) {
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
        if (isPGP(msg.get())) {
            protoName = GpgME::OpenPGP;
        } else if (isSMIME(msg.get())) {
            protoName = GpgME::CMS;
        } else if (isInlinePGP(msg.get())) {
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
    auto inData = multipart ? msg->encodedContent() : msg->decodedBody(); // decodedContent in fact returns decoded body
    auto decrypt = proto->decryptJob();
    if (inlinePGP) {
        auto ctx = QGpgME::Job::context(decrypt);
        ctx->setDecryptionFlags(GpgME::Context::DecryptUnwrap);
    }
    auto result = decrypt->exec(inData, outData);
    if (result.error()) {
        // unknown key, invalid algo, or general error
#if GPGME_VERSION_NUMBER >= 0x011800 // 1.24.0
        qCWarning(MAILCOMMON_LOG) << "Failed to decrypt:" << result.error().asStdString();
#else
        qCWarning(MAILCOMMON_LOG) << "Failed to decrypt:" << result.error().asString();
#endif
        return {};
    }

    if (inlinePGP) {
        inData = outData;
        auto verify = proto->verifyOpaqueJob(true);
        auto resultVerify = verify->exec(inData, outData);
        if (resultVerify.error()) {
#if GPGME_VERSION_NUMBER >= 0x011800 // 1.24.0
            qCWarning(MAILCOMMON_LOG) << "Failed to verify:" << resultVerify.error().asStdString();
#else
            qCWarning(MAILCOMMON_LOG) << "Failed to verify:" << resultVerify.error().asString();
#endif
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

void CryptoUtils::copyHeader(const KMime::Headers::Base *header, std::shared_ptr<KMime::Message> msg)
{
    auto newHdr = KMime::Headers::createHeader(header->type());
    if (!newHdr) {
        newHdr = std::make_unique<KMime::Headers::Generic>(header->type());
    }
    newHdr->from7BitString(header->as7BitString());
    msg->appendHeader(std::move(newHdr));
}

bool CryptoUtils::isContentHeader(const KMime::Headers::Base *header)
{
    return header->is("Content-Type") || header->is("Content-Transfer-Encoding") || header->is("Content-Disposition");
}

std::shared_ptr<KMime::Message> CryptoUtils::assembleMessage(const std::shared_ptr<KMime::Message> &orig, const KMime::Content *newContent)
{
    auto out = std::make_shared<KMime::Message>();
    // Use the new content as message content
    out->setBody(newContent->encodedBody());
    out->parse();

    // remove default explicit content headers added by KMime::Content::parse()
    QList<KMime::Headers::Base *> headers = out->headers();
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
    const auto newContentHeaders = newContent->headers();
    for (const auto hdr : newContentHeaders) {
        if (isContentHeader(hdr)) {
            copyHeader(hdr, out);
        }
    }

    out->assemble();
    out->parse();

    return out;
}
