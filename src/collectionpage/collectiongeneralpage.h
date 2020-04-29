/*
  Copyright (c) 2009-2020 Laurent Montel <montel@kde.org>

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

#ifndef MAILCOMMON_COLLECTIONGENERALPAGE_H
#define MAILCOMMON_COLLECTIONGENERALPAGE_H

#include "mailcommon_export.h"

#include <CollectionPropertiesPage>

class QLineEdit;

class QCheckBox;

template<typename T> class QSharedPointer;
namespace PimCommon {
class IncidencesForWidget;
class ContentTypeWidget;
}
namespace KIdentityManagement {
class IdentityCombo;
}
class QVBoxLayout;
namespace MailCommon {
class FolderSettings;
/**
 * @brief The CollectionGeneralPage class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT CollectionGeneralPage : public Akonadi::CollectionPropertiesPage
{
    Q_OBJECT

public:
    explicit CollectionGeneralPage(QWidget *parent = nullptr);
    ~CollectionGeneralPage() override;

    void load(const Akonadi::Collection &collection) override;
    void save(Akonadi::Collection &collection) override;

protected:
    void init(const Akonadi::Collection &);

private Q_SLOTS:
    void slotIdentityCheckboxChanged();
    void slotFolderContentsSelectionChanged(int);
    void slotNameChanged(const QString &name);

private:
    void addLine(QWidget *parent, QVBoxLayout *layout);
    QString mColorName;
    PimCommon::ContentTypeWidget *mContentsComboBox = nullptr;
    PimCommon::IncidencesForWidget *mIncidencesForComboBox = nullptr;
    QCheckBox *mSharedSeenFlagsCheckBox = nullptr;
    QCheckBox *mNotifyOnNewMailCheckBox = nullptr;
    QCheckBox *mKeepRepliesInSameFolderCheckBox = nullptr;
    QCheckBox *mHideInSelectionDialogCheckBox = nullptr;
    QCheckBox *mUseDefaultIdentityCheckBox = nullptr;
    QLineEdit *mNameEdit = nullptr;
    KIdentityManagement::IdentityCombo *mIdentityComboBox = nullptr;
    QSharedPointer<MailCommon::FolderSettings> mFolderCollection;
    bool mIsLocalSystemFolder = false;
    bool mIsResourceFolder = false;
};

AKONADI_COLLECTION_PROPERTIES_PAGE_FACTORY(CollectionGeneralPageFactory, CollectionGeneralPage)
}

#endif
