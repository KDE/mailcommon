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
// FilterActionRedirect - redirect to
// Redirect message to another user
//=============================================================================
class MAILCOMMON_TESTS_EXPORT FilterActionRedirect : public FilterActionWithAddress
{
    Q_OBJECT
public:
    explicit FilterActionRedirect(QObject *parent = nullptr);
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    [[nodiscard]] QString sieveCode() const override;
    [[nodiscard]] QString informationAboutNotValidAction() const override;
};
}
