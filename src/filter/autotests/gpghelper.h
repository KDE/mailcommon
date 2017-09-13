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

#ifndef GPGHELPER_H_
#define GPGHELPER_H_

#include <QTemporaryDir>

class GPGHelper
{
public:
    enum CryptoType {
        OpenPGP,
        SMIME
    };

    explicit GPGHelper(const QString &templateGnupgHome);
    ~GPGHelper();

    bool isValid() const { return mValid; }
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

