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

#include <QProcess>
#include <QStandardPaths>
#include <QRegularExpression>

using namespace MailCommon;

QStringList FilterActionWithCrypto::getEncryptionKeysFromContent(const KMime::Message::Ptr &msg,
                                                                 GpgME::Protocol protocol) const
{
    if (protocol == GpgME::CMS && mGpgSmPath.isNull()) {
        const auto path = QStandardPaths::findExecutable(QStringLiteral("gpgsm"));
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

