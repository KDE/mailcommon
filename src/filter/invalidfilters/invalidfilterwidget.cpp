/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

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
    , mInvalidFilterListWidget(new InvalidFilterListView(this))
{
    auto vbox = new QVBoxLayout(this);
    vbox->setContentsMargins({});
    auto lab =
        new QLabel(i18n("The following filters are invalid (e.g. containing no actions "
                        "or no search rules). Discard or edit invalid filters?"));
    lab->setWordWrap(true);
    lab->setObjectName(QLatin1StringView("label"));
    vbox->addWidget(lab);

    mInvalidFilterListWidget->setObjectName(QLatin1StringView("invalidfilterlist"));
    connect(mInvalidFilterListWidget, &InvalidFilterListView::showDetails, this, &InvalidFilterWidget::showDetails);
    connect(mInvalidFilterListWidget, &InvalidFilterListView::hideInformationWidget, this, &InvalidFilterWidget::hideInformationWidget);
    vbox->addWidget(mInvalidFilterListWidget);
}

InvalidFilterWidget::~InvalidFilterWidget() = default;

void InvalidFilterWidget::setInvalidFilters(const QList<InvalidFilterInfo> &lst)
{
    mInvalidFilterListWidget->setInvalidFilters(lst);
}

#include "moc_invalidfilterwidget.cpp"
