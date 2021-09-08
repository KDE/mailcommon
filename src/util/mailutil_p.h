/*
  SPDX-FileCopyrightText: 2005 Till Adam <adam@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include "mailutil.h"

#include <Akonadi/AgentInstance>
#include <Akonadi/Collection>

namespace MailCommon
{
/**
 * The Util namespace contains a collection of helper functions use in
 * various places.
 */
namespace Util
{
Q_REQUIRED_RESULT bool ignoreNewMailInFolder(const Akonadi::Collection &collection);
}
}

