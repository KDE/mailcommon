/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionpipethrough.h"

#include <KLocalizedString>

using namespace MailCommon;

FilterAction *FilterActionPipeThrough::newAction()
{
    return new FilterActionPipeThrough;
}

FilterActionPipeThrough::FilterActionPipeThrough(QObject *parent)
    : FilterActionWithCommand(QStringLiteral("filter app"), i18nc("pipe through with command", "Pipe Through"), parent)
{
}

FilterAction::ReturnCode FilterActionPipeThrough::process(ItemContext &context, bool) const
{
    return FilterActionWithCommand::genericProcess(context, true); // use output
}

SearchRule::RequiredPart FilterActionPipeThrough::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

QString FilterActionPipeThrough::informationAboutNotValidAction() const
{
    return i18n("No action defined.");
}
