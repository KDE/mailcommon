/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "invalidfilterlistmodel.h"

using namespace MailCommon;

InvalidFilterListModel::InvalidFilterListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

InvalidFilterListModel::~InvalidFilterListModel()
{
}

bool InvalidFilterListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row > rowCount()) {
        return false;
    }

    if (count <= 0) {
        count = 1;
    }

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        MailCommon::InvalidFilterInfo info;
        mInvalidFilterItems.insert(row, info);
    }
    endInsertRows();

    return true;
}

bool InvalidFilterListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    const int row = index.row();
    if (row >= rowCount()) {
        return false;
    }

    switch (role) {
    case Qt::DisplayRole:
        mInvalidFilterItems[row].setName(value.toString());
        break;
    case InformationRole:
        mInvalidFilterItems[row].setInformation(value.toString());
        break;
    default:
        return false;
    }

    Q_EMIT dataChanged(index, index);
    return true;
}

QVariant InvalidFilterListModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row();
    if (row < rowCount()) {
        switch (role) {
        case Qt::DisplayRole:
            return mInvalidFilterItems[row].name();
        case InformationRole:
            return mInvalidFilterItems[row].information();
        default:
            break;
        }
    }

    return QVariant();
}

int InvalidFilterListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mInvalidFilterItems.count();
}
