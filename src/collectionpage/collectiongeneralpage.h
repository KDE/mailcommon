/*
  SPDX-FileCopyrightText: 2009-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"

#include <Akonadi/CollectionPropertiesPage>
#include <QSharedPointer>

class QLineEdit;

class QCheckBox;

namespace PimCommon
{
class IncidencesForWidget;
class ContentTypeWidget;
}
namespace MailCommon
{
class FolderSettings;
class CollectionGeneralWidget;
/*!
 * \class MailCommon::CollectionGeneralPage
 * \inmodule MailCommon
 * \inheaderfile MailCommon/CollectionGeneralPage
 *
 * \brief The CollectionGeneralPage class.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT CollectionGeneralPage : public Akonadi::CollectionPropertiesPage
{
    Q_OBJECT

public:
    /*!
     * Constructs the general page with the given \a parent.
     */
    explicit CollectionGeneralPage(QWidget *parent = nullptr);
    /*!
     * Destroys the general page.
     */
    ~CollectionGeneralPage() override;

    /*!
     * Loads the general settings of \a collection.
     */
    void load(const Akonadi::Collection &collection) override;
    /*!
     * Saves the general settings into \a collection.
     */
    void save(Akonadi::Collection &collection) override;

protected:
    /*!
     * Initializes the page for \a collection.
     */
    void init(const Akonadi::Collection &collection);

private:
    MAILCOMMON_NO_EXPORT void slotFolderContentsSelectionChanged(int);
    MAILCOMMON_NO_EXPORT void slotNameChanged(const QString &name);

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
