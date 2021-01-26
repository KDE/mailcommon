/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONSENDFAKEDISPOSITION_H
#define MAILCOMMON_FILTERACTIONSENDFAKEDISPOSITION_H

#include "filteractionwithstringlist.h"

namespace MailCommon
{
//=============================================================================
// FilterActionSendFakeDisposition - send fake MDN
// Sends a fake MDN or forces an ignore.
//=============================================================================
class FilterActionSendFakeDisposition : public FilterActionWithStringList
{
    Q_OBJECT
public:
    explicit FilterActionSendFakeDisposition(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;

    static FilterAction *newAction();

    Q_REQUIRED_RESULT bool isEmpty() const override;

    void argsFromString(const QString &argsStr) override;
    Q_REQUIRED_RESULT QString argsAsString() const override;
    Q_REQUIRED_RESULT QString displayString() const override;
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;
};
}

#endif
