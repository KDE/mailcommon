/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "invalidfilterwidget.h"
#include "invalidfilterlistview.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

using namespace MailCommon;

InvalidFilterWidget::InvalidFilterWidget(QWidget *parent)
    : QWidget(parent)
{
    auto vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(0, 0, 0, 0);
    auto lab =
        new QLabel(i18n("The following filters are invalid (e.g. containing no actions "
                        "or no search rules). Discard or edit invalid filters?"));
    lab->setWordWrap(true);
    lab->setObjectName(QStringLiteral("label"));
    vbox->addWidget(lab);

    mInvalidFilterListWidget = new InvalidFilterListView(this);
    mInvalidFilterListWidget->setObjectName(QStringLiteral("invalidfilterlist"));
    connect(mInvalidFilterListWidget, &InvalidFilterListView::showDetails, this, &InvalidFilterWidget::showDetails);
    connect(mInvalidFilterListWidget, &InvalidFilterListView::hideInformationWidget, this, &InvalidFilterWidget::hideInformationWidget);
    vbox->addWidget(mInvalidFilterListWidget);
}

InvalidFilterWidget::~InvalidFilterWidget()
{
}

void InvalidFilterWidget::setInvalidFilters(const QVector<InvalidFilterInfo> &lst)
{
    mInvalidFilterListWidget->setInvalidFilters(lst);
}
