/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "itemcontext.h"

using namespace MailCommon;

ItemContext::ItemContext(const Akonadi::Item &item, bool needsFullPayload)
    : mItem(item)
    , mItemContextAction(needsFullPayload ? FullPayload : None)
{
}

Akonadi::Item &ItemContext::item()
{
    return mItem;
}

void ItemContext::setMoveTargetCollection(const Akonadi::Collection &collection)
{
    mMoveTargetCollection = collection;
}

Akonadi::Collection ItemContext::moveTargetCollection() const
{
    return mMoveTargetCollection;
}

void ItemContext::setNeedsPayloadStore()
{
    mItemContextAction |= PlayloadStore;
}

bool ItemContext::needsPayloadStore() const
{
    return mItemContextAction & PlayloadStore;
}

void ItemContext::setNeedsFlagStore()
{
    mItemContextAction |= FlagStore;
}

bool ItemContext::needsFlagStore() const
{
    return mItemContextAction & FlagStore;
}

void ItemContext::setDeleteItem()
{
    mItemContextAction |= DeleteItem;
}

bool ItemContext::deleteItem() const
{
    return mItemContextAction & DeleteItem;
}

bool ItemContext::needsFullPayload() const
{
    return mItemContextAction & FullPayload;
}
