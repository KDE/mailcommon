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

#ifndef FILTERACTIONDECRYPTTEST_H_
#define FILTERACTIONDECRYPTTEST_H_

#include <QObject>

#include "gpghelper.h"

class FilterActionDecryptTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void shouldDecrypt_data();
    void shouldDecrypt();

private:
    GPGHelper *mGpg = {};
};

#endif

