/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONREDIRECT_H
#define MAILCOMMON_FILTERACTIONREDIRECT_H

#include "filteractionwithaddress.h"

namespace MailCommon
{
//=============================================================================
// FilterActionRedirect - redirect to
// Redirect message to another user
//=============================================================================
class FilterActionRedirect : public FilterActionWithAddress
{
    Q_OBJECT
public:
    explicit FilterActionRedirect(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    Q_REQUIRED_RESULT QString sieveCode() const override;
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;
};
}

#endif
