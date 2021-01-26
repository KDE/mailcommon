/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "invalidfilterlistitemdelegate.h"
#include "invalidfilterlistmodel.h"

#include <QAbstractItemView>
#include <QIcon>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QToolButton>

using namespace MailCommon;

InvalidFilterListItemDelegate::InvalidFilterListItemDelegate(QAbstractItemView *itemView, QObject *parent)
    : KWidgetItemDelegate(itemView, parent)
{
}

InvalidFilterListItemDelegate::~InvalidFilterListItemDelegate()
{
}

QSize InvalidFilterListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    const QStyle *style = itemView()->style();
    const int buttonHeight = style->pixelMetric(QStyle::PM_ButtonMargin) * 2 + style->pixelMetric(QStyle::PM_ButtonIconSize);
    const int fontHeight = option.fontMetrics.height();
    return QSize(100, qMax(buttonHeight, fontHeight));
}

void InvalidFilterListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    painter->save();

    itemView()->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    if (option.state & QStyle::State_Selected) {
        painter->setPen(option.palette.highlightedText().color());
    }

    painter->restore();
}

QList<QWidget *> InvalidFilterListItemDelegate::createItemWidgets(const QModelIndex &) const
{
    auto label = new QLabel();
    auto showInformationToolButton = new QToolButton();
    connect(showInformationToolButton, &QAbstractButton::clicked, this, &InvalidFilterListItemDelegate::slotShowDetails);
    return QList<QWidget *>() << label << showInformationToolButton;
}

void InvalidFilterListItemDelegate::updateItemWidgets(const QList<QWidget *> widgets,
                                                      const QStyleOptionViewItem &option,
                                                      const QPersistentModelIndex &index) const
{
    auto label = static_cast<QLabel *>(widgets[0]);
    const QAbstractItemModel *model = index.model();
    label->setText(model->data(index).toString());
    const QString details = model->data(index, InvalidFilterListModel::InformationRole).toString();

    auto showInformationToolButton = static_cast<QToolButton *>(widgets[1]);
    showInformationToolButton->setEnabled(!details.isEmpty());
    const int itemHeight = sizeHint(option, index).height();
    showInformationToolButton->setIcon(QIcon::fromTheme(QStringLiteral("help-hint")));
    showInformationToolButton->resize(showInformationToolButton->sizeHint());
    showInformationToolButton->move(option.rect.right() - showInformationToolButton->width(), (itemHeight - showInformationToolButton->height()) / 2);

    int labelWidth = option.rect.width();
    labelWidth -= showInformationToolButton->sizeHint().width();
    label->resize(labelWidth, label->sizeHint().height());
    label->move(0, (itemHeight - label->height()) / 2);
}

void InvalidFilterListItemDelegate::slotShowDetails()
{
    const QAbstractItemModel *model = focusedIndex().model();

    const QString information = model->data(focusedIndex(), InvalidFilterListModel::InformationRole).toString();
    if (!information.isEmpty()) {
        Q_EMIT showDetails(information);
    }
}
