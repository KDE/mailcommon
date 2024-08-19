/*
  SPDX-FileCopyrightText: 2009-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"

#include <Akonadi/Collection>
#include <Akonadi/EntityRightsFilterModel>
#include <memory>
namespace Akonadi
{
class AccountActivitiesAbstract;
}
namespace MailCommon
{
/**
 * @brief The FolderTreeWidgetProxyModel class
 * @author Laurent Montel <montel@kde.org>
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

    explicit FolderTreeWidgetProxyModel(QObject *parent = nullptr, FolderTreeWidgetProxyModelOptions = FolderTreeWidgetProxyModel::None);

    ~FolderTreeWidgetProxyModel() override;

    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setEnabledCheck(bool enable);
    [[nodiscard]] bool enabledCheck() const;

    void setHideVirtualFolder(bool exclude);
    [[nodiscard]] bool hideVirtualFolder() const;

    void setHideSpecificFolder(bool hide);
    [[nodiscard]] bool hideSpecificFolder() const;

    void setHideOutboxFolder(bool hide);
    [[nodiscard]] bool hideOutboxFolder() const;

    void addContentMimeTypeInclusionFilter(const QString &mimeTypes);

    void updatePalette();
    void readConfig();

    void setWarningThreshold(qreal threshold);

    void setAccountActivities(Akonadi::AccountActivitiesAbstract *accountActivities);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    class FolderTreeWidgetProxyModelPrivate;
    std::unique_ptr<FolderTreeWidgetProxyModelPrivate> const d;
};
}
