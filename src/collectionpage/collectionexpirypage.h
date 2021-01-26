/*
  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>
  SPDX-FileCopyrightText: 2013 Jonathan Marten <jjm@keelhaul.me.uk>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef MAILCOMMON_COLLECTIONEXPIRYPAGE_H
#define MAILCOMMON_COLLECTIONEXPIRYPAGE_H

#include "mailcommon_export.h"

#include <Collection>
#include <CollectionPropertiesPage>

namespace MailCommon
{
class CollectionExpiryWidget;
/**
 * @brief The CollectionExpiryPage class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT CollectionExpiryPage : public Akonadi::CollectionPropertiesPage
{
    Q_OBJECT

public:
    explicit CollectionExpiryPage(QWidget *parent = nullptr);
    ~CollectionExpiryPage() override;

    Q_REQUIRED_RESULT bool canHandle(const Akonadi::Collection &col) const override;
    void load(const Akonadi::Collection &collection) override;
    void save(Akonadi::Collection &collection) override;

private:
    void saveAndExpire(Akonadi::Collection &collection, bool saveSettings, bool _expirenow);
    void init();
    void slotChanged();
    void slotSaveAndExpire();
    void slotConfigChanged(bool changed);
    CollectionExpiryWidget *mCollectionExpiryWidget = nullptr;
    Akonadi::Collection mCollection;
    bool mChanged = false;
};

AKONADI_COLLECTION_PROPERTIES_PAGE_FACTORY(CollectionExpiryPageFactory, CollectionExpiryPage)
}

#endif
