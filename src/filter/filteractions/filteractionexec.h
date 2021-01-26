/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONEXEC_H
#define MAILCOMMON_FILTERACTIONEXEC_H

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
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;
};
}

#endif
