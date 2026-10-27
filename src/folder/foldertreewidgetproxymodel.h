/*
  SPDX-FileCopyrightText: 2009-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"

#include <Akonadi/EntityRightsFilterModel>
#include <memory>
namespace Akonadi
{
class AccountActivitiesAbstract;
}
namespace MailCommon
{
/*!
 * \class MailCommon::FolderTreeWidgetProxyModel
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FolderTreeWidgetProxyModel
 *
 * \brief The FolderTreeWidgetProxyModel class.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FolderTreeWidgetProxyModel : public Akonadi::EntityRightsFilterModel
{
    Q_OBJECT

public:
    enum FolderTreeWidgetProxyModelOption {
        None = 0,
        HideVirtualFolder = 1,
        HideSpecificFolder = 2,
        HideOutboxFolder = 4,
    };
    Q_DECLARE_FLAGS(FolderTreeWidgetProxyModelOptions, FolderTreeWidgetProxyModelOption)

    /*!
     * Constructs a folder tree proxy model with the given \a parent and \a options.
     */
    explicit FolderTreeWidgetProxyModel(QObject *parent = nullptr, FolderTreeWidgetProxyModelOptions options = FolderTreeWidgetProxyModel::None);

    /*!
     * Destroys the model.
     */
    ~FolderTreeWidgetProxyModel() override;

    /*!
     * Returns the item flags for the given \a index. Folders of broken resources are disabled when the check is enabled.
     */
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    /*!
     * Returns the data for the given \a index and \a role.
     */
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /*!
     * Enables the check of the access rights and of the resource state if \a enable is \c true.
     */
    void setEnabledCheck(bool enable);
    /*!
     * Returns \c true if the check of the access rights and of the resource state is enabled.
     */
    [[nodiscard]] bool enabledCheck() const;

    /*!
     * Hides the virtual folders if \a exclude is \c true.
     */
    void setHideVirtualFolder(bool exclude);
    /*!
     * Returns \c true if the virtual folders are hidden.
     */
    [[nodiscard]] bool hideVirtualFolder() const;

    /*!
     * Hides the folders configured to be hidden in selection dialogs if \a hide is \c true.
     */
    void setHideSpecificFolder(bool hide);
    /*!
     * Returns \c true if the folders configured to be hidden in selection dialogs are hidden.
     */
    [[nodiscard]] bool hideSpecificFolder() const;

    /*!
     * Hides the outbox folder if \a hide is \c true.
     */
    void setHideOutboxFolder(bool hide);
    /*!
     * Returns \c true if the outbox folder is hidden.
     */
    [[nodiscard]] bool hideOutboxFolder() const;

    /*!
     * Only shows the collections that can contain items of the MIME type \a mimeTypes.
     */
    void addContentMimeTypeInclusionFilter(const QString &mimeTypes);

    /*!
     * Updates the palette.
     */
    void updatePalette();
    /*!
     * Reads the configuration and re-filters the model.
     */
    void readConfig();

    /*!
     * Sets the quota usage \a threshold, in percent, above which folders are highlighted.
     */
    void setWarningThreshold(qreal threshold);

    /*!
     * Sets \a accountActivities, used to filter the folders by activity.
     */
    void setAccountActivities(Akonadi::AccountActivitiesAbstract *accountActivities);

protected:
    /*!
     * Returns \c true if the row \a sourceRow under \a sourceParent is accepted.
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    MAILCOMMON_NO_EXPORT void slotInvalidateFilter();
    class FolderTreeWidgetProxyModelPrivate;
    std::unique_ptr<FolderTreeWidgetProxyModelPrivate> const d;
};
}
