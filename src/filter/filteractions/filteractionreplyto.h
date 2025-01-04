/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithaddress.h"
#include "mailcommon_private_export.h"
namespace MailCommon
{
//=============================================================================
// FilterActionReplyTo - set Reply-To to
// Set the Reply-to header in a message
//=============================================================================
class MAILCOMMON_TESTS_EXPORT FilterActionReplyTo : public FilterActionWithAddress
{
    Q_OBJECT
public:
    explicit FilterActionReplyTo(QObject *parent = nullptr);
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    [[nodiscard]] QString informationAboutNotValidAction() const override;
};
}
