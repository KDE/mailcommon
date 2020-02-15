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

#include "gpghelper.h"

#include <QFileInfo>
#include <QDebug>
#include <QProcess>
#include <QTest>

namespace {
bool copyRecursively(const QString &src, const QString &dest)
{
    QFileInfo srcInfo(src);
    if (srcInfo.isDir()) {
        QDir destDir(dest);
        destDir.cdUp();
        if (!destDir.mkdir(QFileInfo(src).fileName())) {
            qWarning() << "Failed to create directory" << QFileInfo(src).fileName() << "in" << destDir.path();
            return false;
        }
        QDir srcDir(src);
        const auto srcFiles = srcDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        for (const auto &fileName : srcFiles) {
            const QString srcFile = src + QLatin1Char('/') + fileName;
            const QString dstFile = dest + QLatin1Char('/') + fileName;
            if (!copyRecursively(srcFile, dstFile)) {
                return false;
            }
        }
    } else {
        if (!QFile::copy(src, dest)) {
            qWarning() << "Failed to copy" << src << "into" << dest;
            return false;
        }
    }
    return true;
}

QString gpgexe(GPGHelper::CryptoType crypto)
{
    return (crypto == GPGHelper::OpenPGP) ? QStringLiteral("gpg2") : QStringLiteral("gpgsm");
}
} // namespace

GPGHelper::GPGHelper(const QString &templateGnupgHome)
    : mValid(false)
{
    const auto home = gnupgHome();
    mValid = copyRecursively(templateGnupgHome, home);
    if (mValid) {
        qputenv("GNUPGHOME", home.toUtf8());
    }
}

GPGHelper::~GPGHelper()
{
    // shutdown gpg-agent
    QProcess gpgshutdown;
    auto env = gpgshutdown.processEnvironment();
    env.insert(QStringLiteral("GNUPGHOME"), gnupgHome());
    gpgshutdown.setProcessEnvironment(env);
    gpgshutdown.start(QStringLiteral("gpg-connect-agent"), QStringList());
    QVERIFY(gpgshutdown.waitForStarted());
    gpgshutdown.write("KILLAGENT");
    gpgshutdown.closeWriteChannel();
    QVERIFY(gpgshutdown.waitForFinished());
}

QString GPGHelper::gnupgHome() const
{
    return mTmpDir.path() + QStringLiteral("/gpghome");
}

QByteArray GPGHelper::runGpg(const QByteArray &in, GPGHelper::CryptoType crypto, const QStringList &args) const
{
    QProcess gpg;
    gpg.setReadChannel(QProcess::StandardOutput);
    auto env = gpg.processEnvironment();
    env.insert(QStringLiteral("GNUPGHOME"), gnupgHome());
    gpg.setProcessEnvironment(env);
    gpg.start(gpgexe(crypto), args);
    if (!gpg.waitForStarted()) {
        return {};
    }
    gpg.write(in);
    gpg.closeWriteChannel();
    if (!gpg.waitForReadyRead()) {
        return {};
    }
    const auto out = gpg.readAllStandardOutput();

    if (!gpg.waitForFinished()) {
        return {};
    }

    return out;
}

QByteArray GPGHelper::decrypt(const QByteArray &enc, GPGHelper::CryptoType crypto) const
{
    return runGpg(enc, crypto, { QStringLiteral("-d") });
}

QByteArray GPGHelper::encrypt(const QByteArray &dec, GPGHelper::CryptoType crypto) const
{
    return runGpg(dec, crypto, { QStringLiteral("-e") });
}

QString GPGHelper::encryptionKeyFp(const QByteArray &enc, GPGHelper::CryptoType crypto) const
{
    const auto data = runGpg(enc, crypto, { QStringLiteral("--fingerprint"),
                                            QStringLiteral("--with-colons") });
    int idx = data.indexOf("\nfpr:");
    if (idx == -1) {
        return {};
    }

    // Find first non-colon character after "fpr"
    for (idx = idx + 4; idx < data.size() && data[idx] == ':'; ++idx) {
    }
    const int end = data.indexOf(':', idx);

    return QString::fromLatin1(data.constData() + idx, end - idx);
}
