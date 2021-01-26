/*
 *  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only
 */

#ifndef GPGHELPER_H_
#define GPGHELPER_H_

#include <QTemporaryDir>

class GPGHelper
{
public:
    enum CryptoType { OpenPGP, SMIME };

    explicit GPGHelper(const QString &templateGnupgHome);
    ~GPGHelper();

    bool isValid() const
    {
        return mValid;
    }

    QString gnupgHome() const;

    QByteArray decrypt(const QByteArray &enc, CryptoType crypto) const;
    QByteArray encrypt(const QByteArray &dec, CryptoType crypto) const;

    QString encryptionKeyFp(const QByteArray &encMsg, GPGHelper::CryptoType crypto) const;

private:
    QByteArray runGpg(const QByteArray &in, CryptoType crypt, const QStringList &args) const;

    bool mValid;
    QTemporaryDir mTmpDir;
};

#endif
