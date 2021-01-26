/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef INVALIDFILTERLISTMODEL_H
#define INVALIDFILTERLISTMODEL_H

#include "invalidfilterinfo.h"
#include <QAbstractListModel>
#include <QVector>
namespace MailCommon
{
class InvalidFilterListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Role { InformationRole = Qt::UserRole + 1 };

    explicit InvalidFilterListModel(QObject *parent = nullptr);
    ~InvalidFilterListModel() override;

    Q_REQUIRED_RESULT bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    Q_REQUIRED_RESULT bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    QVector<MailCommon::InvalidFilterInfo> mInvalidFilterItems;
};
}

#endif // INVALIDFILTERLISTMODEL_H
