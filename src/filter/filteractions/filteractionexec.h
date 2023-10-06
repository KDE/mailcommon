/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithcommand.h"

namespace MailCommon
{
//=============================================================================
// FilterActionExec - execute command
// Execute a shell command
//=============================================================================
class FilterActionExec : public FilterActionWithCommand
{
    Q_OBJECT
public:
    explicit FilterActionExec(QObject *parent = nullptr);
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    [[nodiscard]] QString informationAboutNotValidAction() const override;
};
}
