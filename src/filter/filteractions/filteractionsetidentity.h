/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithuoid.h"

namespace MailCommon
{
//=============================================================================
// FilterActionSetIdentity - set identity to
// Specify Identity to be used when replying to a message
//=============================================================================
class FilterActionSetIdentity : public FilterActionWithUOID
{
    Q_OBJECT
public:
    explicit FilterActionSetIdentity(QObject *parent = nullptr);
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    [[nodiscard]] bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;
    static FilterAction *newAction();

    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    void applyParamWidgetValue(QWidget *parent) override;
    void setParamWidgetValue(QWidget *parent) const override;
    void clearParamWidget(QWidget *param) const override;
};
}
