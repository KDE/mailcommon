/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "collectionpage/attributes/expirecollectionattribute.h"
#include <Akonadi/KMime/NewMailNotifierAttribute>

#include <AkonadiCore/attributefactory.h>

namespace
{
// Anonymous namespace; function is invisible outside this file.
bool dummy()
{
    Akonadi::AttributeFactory::registerAttribute<MailCommon::ExpireCollectionAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::NewMailNotifierAttribute>();
    return true;
}

// Called when this library is loaded.
const bool registered = dummy();
} // namespace
