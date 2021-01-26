/*
  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_COLLECTIONGENERALPAGE_H
#define MAILCOMMON_COLLECTIONGENERALPAGE_H

#include "mailcommon_export.h"

#include <CollectionPropertiesPage>

class QLineEdit;

class QCheckBox;

template<typename T> class QSharedPointer;
namespace PimCommon
{
class IncidencesForWidget;
class ContentTypeWidget;
}
namespace KIdentityManagement
{
}
namespace MailCommon
{
class FolderSettings;
class CollectionGeneralWidget;
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
    void slotFolderContentsSelectionChanged(int);
    void slotNameChanged(const QString &name);

private:
    QString mColorName;

    CollectionGeneralWidget *mCollectionGeneralWidget = nullptr;
    PimCommon::ContentTypeWidget *mContentsComboBox = nullptr;
    PimCommon::IncidencesForWidget *mIncidencesForComboBox = nullptr;
    QCheckBox *mSharedSeenFlagsCheckBox = nullptr;
    QLineEdit *mNameEdit = nullptr;
    QSharedPointer<MailCommon::FolderSettings> mFolderCollection;
    bool mIsLocalSystemFolder = false;
    bool mIsResourceFolder = false;
};

AKONADI_COLLECTION_PROPERTIES_PAGE_FACTORY(CollectionGeneralPageFactory, CollectionGeneralPage)
}

#endif
