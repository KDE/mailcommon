/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithnone.h"

namespace MailCommon
{
//=============================================================================
// FilterActionDelete - Delete action
//=============================================================================
class FilterActionDelete : public FilterActionWithNone
{
    Q_OBJECT
public:
    explicit FilterActionDelete(QObject *parent = nullptr);
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    [[nodiscard]] QString sieveCode() const override;
};
}
