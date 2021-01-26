/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "invalidfilterlistview.h"
#include "invalidfilterlistitemdelegate.h"
#include "invalidfilterlistmodel.h"
using namespace MailCommon;

InvalidFilterListView::InvalidFilterListView(QWidget *parent)
    : QListView(parent)
{
    auto invalidFilterDelegate = new InvalidFilterListItemDelegate(this, this);

    auto invalidFilterListModel = new InvalidFilterListModel(this);
    connect(invalidFilterDelegate, &InvalidFilterListItemDelegate::showDetails, this, &InvalidFilterListView::showDetails);
    // connect(this, SIGNAL(pressed(QModelIndex)), SIGNAL(hideInformationWidget()));
    setModel(invalidFilterListModel);
    setItemDelegate(invalidFilterDelegate);
}

InvalidFilterListView::~InvalidFilterListView()
{
}

void InvalidFilterListView::setInvalidFilters(const QVector<MailCommon::InvalidFilterInfo> &lst)
{
    for (const MailCommon::InvalidFilterInfo &info : lst) {
        model()->insertRow(0);
        const QModelIndex index = model()->index(0, 0);
        model()->setData(index, info.name(), Qt::DisplayRole);
        model()->setData(index, info.information(), InvalidFilterListModel::InformationRole);
    }
    model()->sort(Qt::DisplayRole);
}
