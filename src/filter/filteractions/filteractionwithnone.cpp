/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionwithnone.h"

using namespace MailCommon;

FilterActionWithNone::FilterActionWithNone(const QString &name, const QString &label, QObject *parent)
    : FilterAction(name, label, parent)
{
}

QString FilterActionWithNone::displayString() const
{
    return label();
}

void FilterActionWithNone::argsFromString(const QString &)
{
}

QString FilterActionWithNone::argsAsString() const
{
    return QString();
}
