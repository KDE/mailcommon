/*
  SPDX-FileCopyrightText: 2009-2025 Laurent Montel <montel@kde.org>
  SPDX-FileCopyrightText: 2013 Jonathan Marten <jjm@keelhaul.me.uk>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "collectionexpirypage.h"
#include "collectionexpirywidget.h"

#include "attributes/expirecollectionattribute.h"

#include <KLocalizedString>
#include <QVBoxLayout>

using namespace Akonadi;
using namespace MailCommon;

CollectionExpiryPage::CollectionExpiryPage(QWidget *parent)
    : CollectionPropertiesPage(parent)
{
    setObjectName(QLatin1StringView("MailCommon::CollectionExpiryPage"));
    setPageTitle(i18nc("@title:tab Expiry settings for a folder.", "Expiry"));
}

CollectionExpiryPage::~CollectionExpiryPage() = default;

bool CollectionExpiryPage::canHandle(const Akonadi::Collection &col) const
{
    return CollectionExpiryWidget::canHandle(col);
}

void CollectionExpiryPage::init()
{
    auto globalVBox = new QVBoxLayout(this);
    mCollectionExpiryWidget = new CollectionExpiryWidget(this);
    connect(mCollectionExpiryWidget, &CollectionExpiryWidget::saveAndExpireRequested, this, &CollectionExpiryPage::slotSaveAndExpire);
    connect(mCollectionExpiryWidget, &CollectionExpiryWidget::configChanged, this, &CollectionExpiryPage::slotConfigChanged);
    globalVBox->addWidget(mCollectionExpiryWidget);

    globalVBox->addStretch(100); // eat all superfluous space
}

void CollectionExpiryPage::slotConfigChanged(bool changed)
{
    mChanged = changed;
}

void CollectionExpiryPage::load(const Akonadi::Collection &collection)
{
    mCollection = collection;
    init();
    const auto *attr = collection.attribute<MailCommon::ExpireCollectionAttribute>();
    if (attr) {
        MailCommon::CollectionExpirySettings settings;
        settings.convertFromExpireCollectionAttribute(attr);
        mCollectionExpiryWidget->load(settings);
    } else {
        mCollectionExpiryWidget->load({});
    }
    mChanged = false;
}

void CollectionExpiryPage::save(Akonadi::Collection &collection)
{
    if (mChanged) {
        saveAndExpire(collection, false, true);
    }
}

void CollectionExpiryPage::saveAndExpire(Akonadi::Collection &collection, bool saveSettings, bool expireNow)
{
    mCollectionExpiryWidget->save(collection, saveSettings, expireNow);
    mChanged = false;
}

void CollectionExpiryPage::slotSaveAndExpire()
{
    saveAndExpire(mCollection, true, true); // save and start expire job
}

void CollectionExpiryPage::slotChanged()
{
    mChanged = true;
}

#include "moc_collectionexpirypage.cpp"
