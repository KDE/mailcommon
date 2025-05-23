/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "mailcommon/searchpattern.h"
#include "mailcommon_export.h"

#include <Akonadi/Collection>
#include <Akonadi/Item>

namespace MailCommon
{
/**
 * @short A helper class for the filtering process
 *
 * The item context is used to pass the item together with meta data
 * through the filter chain.
 * This allows to 'record' all actions that shall be taken and execute them
 * at the end of the filter chain.
 */
class MAILCOMMON_EXPORT ItemContext
{
public:
    /**
     * Creates an item context for the given @p item.
     * @p requestedPart the part requested for the item (Envelope, Header or CompleteMessage)
     */
    ItemContext(const Akonadi::Item &item, bool needsFullPayload);

    /**
     * Returns the item of the context.
     */
    Akonadi::Item &item();

    /**
     * Sets the target collection the item should be moved to.
     */
    void setMoveTargetCollection(const Akonadi::Collection &collection);

    /**
     * Returns the target collection the item should be moved to, or an invalid
     * collection if the item should not be moved at all.
     */
    [[nodiscard]] Akonadi::Collection moveTargetCollection() const;

    /**
     * Marks that the item's payload has been changed and needs to be written back.
     */
    void setNeedsPayloadStore();

    /**
     * Returns whether the item's payload needs to be written back.
     */
    [[nodiscard]] bool needsPayloadStore() const;

    /**
     * Marks that the item's flags has been changed and needs to be written back.
     */
    void setNeedsFlagStore();

    /**
     * Returns whether the item's flags needs to be written back.
     */
    [[nodiscard]] bool needsFlagStore() const;

    /** Returns true if the full payload was requested for the item or not.
     * Full payload is needed to change the headers or the body */
    [[nodiscard]] bool needsFullPayload() const;

    void setDeleteItem();
    [[nodiscard]] bool deleteItem() const;

private:
    enum class ItemContextAction : uint8_t {
        None = 0,
        PayloadStore = 1,
        FlagStore = 2,
        DeleteItem = 4,
        FullPayload = 8,
    };
    Q_DECLARE_FLAGS(ItemContextActions, ItemContextAction)

    Akonadi::Item mItem;
    Akonadi::Collection mMoveTargetCollection;
    ItemContextActions mItemContextAction;
};
}
