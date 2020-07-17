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

#ifndef MAILCOMMON_COLLECTIONEXPIRYPAGE_H
#define MAILCOMMON_COLLECTIONEXPIRYPAGE_H

#include "mailcommon_export.h"

#include <Collection>
#include <CollectionPropertiesPage>

class KPluralHandlingSpinBox;
class KJob;

class QCheckBox;
class QPushButton;
class QRadioButton;

namespace MailCommon {
class FolderRequester;
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
