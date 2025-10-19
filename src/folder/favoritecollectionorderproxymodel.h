/*

  SPDX-FileCopyrightText: 2017 David Faure <faure@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/EntityOrderProxyModel>
namespace Akonadi
{
class AccountActivitiesAbstract;
}
namespace MailCommon
{
/**
 * @brief The FavoriteCollectionOrderProxyModel class implements ordering of favorite collections.
 */
class MAILCOMMON_EXPORT FavoriteCollectionOrderProxyModel : public Akonadi::EntityOrderProxyModel
{
    Q_OBJECT
public:
    explicit FavoriteCollectionOrderProxyModel(QObject *parent = nullptr);
    ~FavoriteCollectionOrderProxyModel() override;

    void setAccountActivities(Akonadi::AccountActivitiesAbstract *accountActivities);

protected:
    [[nodiscard]] Akonadi::Collection parentCollection(const QModelIndex &index) const override;
    [[nodiscard]] bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    void slotInvalidateFilter();
    Akonadi::AccountActivitiesAbstract *mAccountActivities = nullptr;
};
}
