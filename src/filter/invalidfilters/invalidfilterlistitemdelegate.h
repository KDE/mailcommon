/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef INVALIDFILTERLISTITEMDELEGATE_H
#define INVALIDFILTERLISTITEMDELEGATE_H

#include <KWidgetItemDelegate>

namespace MailCommon
{
class InvalidFilterListItemDelegate : public KWidgetItemDelegate
{
    Q_OBJECT
public:
    explicit InvalidFilterListItemDelegate(QAbstractItemView *itemView, QObject *parent = nullptr);
    ~InvalidFilterListItemDelegate() override;

    Q_REQUIRED_RESULT QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    Q_REQUIRED_RESULT QList<QWidget *> createItemWidgets(const QModelIndex &) const override;

    void updateItemWidgets(const QList<QWidget *> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const override;
private Q_SLOTS:
    void slotShowDetails();
Q_SIGNALS:
    void showDetails(const QString &details);
};
}
#endif // INVALIDFILTERLISTITEMDELEGATE_H
