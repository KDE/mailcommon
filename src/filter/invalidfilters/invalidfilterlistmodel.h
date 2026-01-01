/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "invalidfilterinfo.h"
#include <QAbstractListModel>
#include <QList>
namespace MailCommon
{
class InvalidFilterListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Role {
        InformationRole = Qt::UserRole + 1
    };

    explicit InvalidFilterListModel(QObject *parent = nullptr);
    ~InvalidFilterListModel() override;

    [[nodiscard]] bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    [[nodiscard]] bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    QList<MailCommon::InvalidFilterInfo> mInvalidFilterItems;
};
}
