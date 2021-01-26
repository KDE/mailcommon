/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONSENDRECEIPT_H
#define MAILCOMMON_FILTERACTIONSENDRECEIPT_H

#include "filteractionwithnone.h"

namespace MailCommon
{
//=============================================================================
// FilterActionSendReceipt - send receipt
// Return delivery receipt.
//=============================================================================
class FilterActionSendReceipt : public FilterActionWithNone
{
    Q_OBJECT
public:
    explicit FilterActionSendReceipt(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
};
}

#endif
