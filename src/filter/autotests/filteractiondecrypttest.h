/*
 *  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

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
