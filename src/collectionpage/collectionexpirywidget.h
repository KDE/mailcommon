/*
  Copyright (c) 2020 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef COLLECTIONEXPIRYWIDGET_H
#define COLLECTIONEXPIRYWIDGET_H
#include "mailcommon_export.h"

#include <MailCommon/ExpireCollectionAttribute>
#include <QWidget>
class QCheckBox;
class KPluralHandlingSpinBox;
class QRadioButton;
class QPushButton;
namespace MailCommon {
class FolderRequester;
struct MAILCOMMON_EXPORT CollectionExpirySettings
{
    Q_REQUIRED_RESULT bool isValid() const {
        return daysToExpireRead != -1 ||
                daysToExpireUnread != -1 ||
                mUnreadExpireUnits != ExpireCollectionAttribute::ExpireNever ||
                mReadExpireUnits != ExpireCollectionAttribute::ExpireNever ||
                mExpireAction != ExpireCollectionAttribute::ExpireDelete ||
                mExpireToFolderId != -1;
    }

    bool expiryGloballyOn = false;
    int daysToExpireRead = -1;
    int daysToExpireUnread = -1;
    ExpireCollectionAttribute::ExpireUnits mUnreadExpireUnits = ExpireCollectionAttribute::ExpireNever;
    ExpireCollectionAttribute::ExpireUnits mReadExpireUnits = ExpireCollectionAttribute::ExpireNever;
    ExpireCollectionAttribute::ExpireAction mExpireAction = ExpireCollectionAttribute::ExpireDelete;
    Akonadi::Collection::Id mExpireToFolderId = -1;
};

class MAILCOMMON_EXPORT CollectionExpiryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CollectionExpiryWidget(QWidget *parent = nullptr);
    ~CollectionExpiryWidget();

    void load(const MailCommon::CollectionExpirySettings &settings);
    Q_REQUIRED_RESULT MailCommon::CollectionExpirySettings save();
Q_SIGNALS:
    void saveAndExpireRequested();
    void configChanged();

private:
    void slotChanged();
    void slotUpdateControls();
    QCheckBox *expireReadMailCB = nullptr;
    KPluralHandlingSpinBox *expireReadMailSB = nullptr;
    QCheckBox *expireUnreadMailCB = nullptr;
    KPluralHandlingSpinBox *expireUnreadMailSB = nullptr;
    QRadioButton *moveToRB = nullptr;
    FolderRequester *folderSelector = nullptr;
    QRadioButton *deletePermanentlyRB = nullptr;
    QPushButton *expireNowPB = nullptr;
};
}

#endif // COLLECTIONEXPIRYWIDGET_H
