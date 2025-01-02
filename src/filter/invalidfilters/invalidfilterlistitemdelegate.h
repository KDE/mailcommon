/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KWidgetItemDelegate>

namespace MailCommon
{
class InvalidFilterListItemDelegate : public KWidgetItemDelegate
{
    Q_OBJECT
public:
    explicit InvalidFilterListItemDelegate(QAbstractItemView *itemView, QObject *parent = nullptr);
    ~InvalidFilterListItemDelegate() override;

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    [[nodiscard]] QList<QWidget *> createItemWidgets(const QModelIndex &) const override;
    void updateItemWidgets(const QList<QWidget *> &widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const override;
Q_SIGNALS:
    void showDetails(const QString &details);

private:
    void slotShowDetails();
};
}
