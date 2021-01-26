/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionexec.h"

#include <KLocalizedString>

using namespace MailCommon;

FilterActionExec::FilterActionExec(QObject *parent)
    : FilterActionWithCommand(QStringLiteral("execute"), i18n("Execute Command"), parent)
{
}

FilterAction::ReturnCode FilterActionExec::process(ItemContext &context, bool) const
{
    return FilterActionWithCommand::genericProcess(context, false); // ignore output
}

SearchRule::RequiredPart FilterActionExec::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

FilterAction *FilterActionExec::newAction()
{
    return new FilterActionExec();
}

QString FilterActionExec::informationAboutNotValidAction() const
{
    return i18n("Missing command.");
}
