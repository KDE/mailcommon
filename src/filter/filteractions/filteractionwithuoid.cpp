/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionwithuoid.h"

using namespace MailCommon;

FilterActionWithUOID::FilterActionWithUOID(const QString &name, const QString &label, QObject *parent)
    : FilterAction(name, label, parent)
    , mParameter(0)
{
}

bool FilterActionWithUOID::isEmpty() const
{
    return mParameter == 0;
}

void FilterActionWithUOID::argsFromString(const QString &argsStr)
{
    bool valid;
    const uint val = argsStr.trimmed().toUInt(&valid);

    mParameter = valid ? val : 0;
}

QString FilterActionWithUOID::argsAsString() const
{
    return QString::number(mParameter);
}

QString FilterActionWithUOID::displayString() const
{
    return label() + QLatin1String(" \"") + argsAsString().toHtmlEscaped() + QLatin1String("\"");
}
