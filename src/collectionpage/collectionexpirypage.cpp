/*
  Copyright (c) 2009-2020 Laurent Montel <montel@kde.org>
  Copyright (c) 2013 Jonathan Marten <jjm@keelhaul.me.uk>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "collectionexpirypage.h"
#include "collectionexpirywidget.h"

#include "attributes/expirecollectionattribute.h"
#include "folder/foldersettings.h"
#include "util/mailutil.h"
#include "kernel/mailkernel.h"
#include "mailcommon_debug.h"

#include <KLocalizedString>
#include <QVBoxLayout>

using namespace Akonadi;
using namespace MailCommon;

CollectionExpiryPage::CollectionExpiryPage(QWidget *parent)
    : CollectionPropertiesPage(parent)
{
    setObjectName(QStringLiteral("MailCommon::CollectionExpiryPage"));
    setPageTitle(i18nc("@title:tab Expiry settings for a folder.", "Expiry"));
}

CollectionExpiryPage::~CollectionExpiryPage()
{
}

bool CollectionExpiryPage::canHandle(const Akonadi::Collection &col) const
{
    QSharedPointer<FolderSettings> fd = FolderSettings::forCollection(col, false);
    return fd->canDeleteMessages() && !fd->isStructural() && !MailCommon::Util::isVirtualCollection(col);
}

void CollectionExpiryPage::init()
{
    QVBoxLayout *globalVBox = new QVBoxLayout(this);
    mCollectionExpiryWidget = new CollectionExpiryWidget(this);
    connect(mCollectionExpiryWidget, &CollectionExpiryWidget::saveAndExpireRequested, this, &CollectionExpiryPage::slotSaveAndExpire);
    connect(mCollectionExpiryWidget, &CollectionExpiryWidget::configChanged, this, &CollectionExpiryPage::slotConfigChanged);
    globalVBox->addWidget(mCollectionExpiryWidget);

    globalVBox->addStretch(100);   // eat all superfluous space
}

void CollectionExpiryPage::slotConfigChanged(bool changed)
{
    mChanged = changed;
}

void CollectionExpiryPage::load(const Akonadi::Collection &collection)
{
    mCollection = collection;
    init();
    const MailCommon::ExpireCollectionAttribute *attr = collection.attribute<MailCommon::ExpireCollectionAttribute>();
    if (attr) {
        int daysToExpireRead, daysToExpireUnread;
        attr->daysToExpire(daysToExpireUnread, daysToExpireRead);
        MailCommon::CollectionExpirySettings settings;
        settings.expiryGloballyOn = attr->isAutoExpire();
        settings.daysToExpireRead = daysToExpireUnread;
        settings.daysToExpireUnread = daysToExpireRead;
        settings.mUnreadExpireUnits = attr->readExpireUnits();
        settings.mReadExpireUnits = attr->unreadExpireUnits();
        settings.mExpireAction = attr->expireAction();
        settings.mExpireToFolderId = attr->expireToFolderId();


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
    saveAndExpire(mCollection, true, true);                          // save and start expire job
}

void CollectionExpiryPage::slotChanged()
{
    mChanged = true;
}
