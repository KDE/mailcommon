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
    , mItemContextAction(needsFullPayload ? ItemContextAction::FullPayload : ItemContextAction::None)
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
    mItemContextAction |= ItemContextAction::PlayloadStore;
}

bool ItemContext::needsPayloadStore() const
{
    return mItemContextAction & ItemContextAction::PlayloadStore;
}

void ItemContext::setNeedsFlagStore()
{
    mItemContextAction |= ItemContextAction::FlagStore;
}

bool ItemContext::needsFlagStore() const
{
    return mItemContextAction & ItemContextAction::FlagStore;
}

void ItemContext::setDeleteItem()
{
    mItemContextAction |= ItemContextAction::DeleteItem;
}

bool ItemContext::deleteItem() const
{
    return mItemContextAction & ItemContextAction::DeleteItem;
}

bool ItemContext::needsFullPayload() const
{
    return mItemContextAction & ItemContextAction::FullPayload;
}
