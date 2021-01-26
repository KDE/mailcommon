/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONPIPETHROUGH_H
#define MAILCOMMON_FILTERACTIONPIPETHROUGH_H

#include "filteractionwithcommand.h"

namespace MailCommon
{
//=============================================================================
// FilterActionPipeThrough - use external filter app
// External message filter: executes a shell command with message
// on stdin; altered message is expected on stdout.
//=============================================================================
class FilterActionPipeThrough : public FilterActionWithCommand
{
    Q_OBJECT
public:
    explicit FilterActionPipeThrough(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;
};
}

#endif
