/*
  Copyright (c) 2009-2020 Laurent Montel <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#ifndef MAILCOMMON_FOLDERTREEWIDGETPROXYMODEL_H
#define MAILCOMMON_FOLDERTREEWIDGETPROXYMODEL_H

#include "mailcommon_export.h"

#include <Collection>
#include <EntityRightsFilterModel>

namespace MailCommon {
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
        HideOutboxFolder = 4
    };
    Q_DECLARE_FLAGS(FolderTreeWidgetProxyModelOptions, FolderTreeWidgetProxyModelOption)

    explicit FolderTreeWidgetProxyModel(
        QObject *parent = nullptr, FolderTreeWidgetProxyModelOptions = FolderTreeWidgetProxyModel::None);

    ~FolderTreeWidgetProxyModel() override;

    Q_REQUIRED_RESULT Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setEnabledCheck(bool enable);
    Q_REQUIRED_RESULT bool enabledCheck() const;

    void setHideVirtualFolder(bool exclude);
    Q_REQUIRED_RESULT bool hideVirtualFolder() const;

    void setHideSpecificFolder(bool hide);
    Q_REQUIRED_RESULT bool hideSpecificFolder() const;

    void setHideOutboxFolder(bool hide);
    Q_REQUIRED_RESULT bool hideOutboxFolder() const;

    void addContentMimeTypeInclusionFilter(const QString &mimeTypes);

    void updatePalette();
    void readConfig();

    void setWarningThreshold(qreal threshold);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    class Private;
    Private *const d;
};
}

#endif
