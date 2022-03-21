/*

  SPDX-FileCopyrightText: 2017 David Faure <faure@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/EntityOrderProxyModel>

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

protected:
    Akonadi::Collection parentCollection(const QModelIndex &index) const override;
};
}
