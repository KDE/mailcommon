/*
 *  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only
 */

#ifndef FILTERACTIONENCRYPTTEST_H_
#define FILTERACTIONENCRYPTTEST_H_

#include "gpghelper.h"
#include <QObject>

class FilterActionEncryptTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void shouldEncrypt_data();
    void shouldEncrypt();

private:
    GPGHelper *mGpg = {};
};

#endif
