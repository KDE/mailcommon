/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONREPLYTO_H
#define MAILCOMMON_FILTERACTIONREPLYTO_H

#include "filteractionwithaddress.h"

namespace MailCommon
{
//=============================================================================
// FilterActionReplyTo - set Reply-To to
// Set the Reply-to header in a message
//=============================================================================
class FilterActionReplyTo : public FilterActionWithAddress
{
    Q_OBJECT
public:
    explicit FilterActionReplyTo(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;
};
}

#endif
