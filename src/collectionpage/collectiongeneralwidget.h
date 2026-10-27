/*
  SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/Collection>
#include <QSharedPointer>
#include <QWidget>
class QCheckBox;
namespace KIdentityManagementWidgets
{
class IdentityCombo;
}
namespace MailCommon
{
class FolderSettings;
/*!
 * \class MailCommon::CollectionGeneralWidget
 * \inmodule MailCommon
 * \inheaderfile MailCommon/CollectionGeneralWidget
 *
 * \brief The CollectionGeneralWidget class.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT CollectionGeneralWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs the general widget with the given \a parent.
     */
    explicit CollectionGeneralWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the general widget.
     */
    ~CollectionGeneralWidget() override;
    /*!
     * Saves the settings shown in the widget into \a collection.
     */
    void save(Akonadi::Collection &collection);
    /*!
     * Loads the settings of the collection \a col into the widget.
     */
    void load(const Akonadi::Collection &col);

private:
    MAILCOMMON_NO_EXPORT void slotIdentityCheckboxChanged();
    QCheckBox *mNotifyOnNewMailCheckBox = nullptr;
    QCheckBox *mKeepRepliesInSameFolderCheckBox = nullptr;
    QCheckBox *mHideInSelectionDialogCheckBox = nullptr;
    QCheckBox *mUseDefaultIdentityCheckBox = nullptr;
    KIdentityManagementWidgets::IdentityCombo *mIdentityComboBox = nullptr;
    QSharedPointer<MailCommon::FolderSettings> mFolderCollection;
};
}
