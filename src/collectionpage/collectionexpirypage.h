/*
  SPDX-FileCopyrightText: 2009-2026 Laurent Montel <montel@kde.org>
  SPDX-FileCopyrightText: 2013 Jonathan Marten <jjm@keelhaul.me.uk>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "mailcommon_export.h"

#include <Akonadi/Collection>
#include <Akonadi/CollectionPropertiesPage>

namespace MailCommon
{
class CollectionExpiryWidget;
/*!
 * \class MailCommon::CollectionExpiryPage
 * \inmodule MailCommon
 * \inheaderfile MailCommon/CollectionExpiryPage
 *
 * \brief The CollectionExpiryPage class.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT CollectionExpiryPage : public Akonadi::CollectionPropertiesPage
{
    Q_OBJECT

public:
    /*!
     * Constructs the expiry page with the given \a parent.
     */
    explicit CollectionExpiryPage(QWidget *parent = nullptr);
    /*!
     * Destroys the expiry page.
     */
    ~CollectionExpiryPage() override;

    /*!
     * Returns \c true if expiry can be configured for the collection \a col.
     */
    [[nodiscard]] bool canHandle(const Akonadi::Collection &col) const override;
    /*!
     * Loads the expiry settings of \a collection.
     */
    void load(const Akonadi::Collection &collection) override;
    /*!
     * Saves the expiry settings into \a collection.
     */
    void save(Akonadi::Collection &collection) override;

private:
    MAILCOMMON_NO_EXPORT void saveAndExpire(Akonadi::Collection &collection, bool saveSettings, bool _expirenow);
    MAILCOMMON_NO_EXPORT void init();
    MAILCOMMON_NO_EXPORT void slotChanged();
    MAILCOMMON_NO_EXPORT void slotSaveAndExpire();
    MAILCOMMON_NO_EXPORT void slotConfigChanged(bool changed);
    CollectionExpiryWidget *mCollectionExpiryWidget = nullptr;
    Akonadi::Collection mCollection;
    bool mChanged = false;
};

AKONADI_COLLECTION_PROPERTIES_PAGE_FACTORY(CollectionExpiryPageFactory, CollectionExpiryPage)
}
