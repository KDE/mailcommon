/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterselectiondialog.h"
#include "mailfilter.h"

#include <KListWidgetSearchLine>
#include <QPushButton>

#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWindow>

using namespace MailCommon;
namespace
{
static const char myFilterSelectionDialogName[] = "FilterSelectionDialog";
}
FilterSelectionDialog::FilterSelectionDialog(QWidget *parent)
    : QDialog(parent)
    , filtersListWidget(new QListWidget(this))
    , selectAllButton(new QPushButton(i18n("Select All"), this))
    , unselectAllButton(new QPushButton(i18n("Unselect All"), this))
{
    setObjectName(QLatin1StringView("filterselection"));
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Filters"));
    auto top = new QVBoxLayout(this);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    mOkButton->setDefault(true);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterSelectionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterSelectionDialog::reject);

    auto searchLine = new KListWidgetSearchLine(this, filtersListWidget);
    searchLine->setPlaceholderText(i18nc("@info Displayed grayed-out inside the textbox, verb to search", "Search"));

    top->addWidget(searchLine);
    top->addWidget(filtersListWidget);
    filtersListWidget->setAlternatingRowColors(true);
    filtersListWidget->setSortingEnabled(false);
    filtersListWidget->setSelectionMode(QAbstractItemView::NoSelection);

    auto const buttonLayout = new QHBoxLayout();
    top->addLayout(buttonLayout);
    buttonLayout->addWidget(selectAllButton);
    buttonLayout->addWidget(unselectAllButton);
    top->addWidget(buttonBox);

    connect(selectAllButton, &QPushButton::clicked, this, &FilterSelectionDialog::slotSelectAllButton);
    connect(unselectAllButton, &QPushButton::clicked, this, &FilterSelectionDialog::slotUnselectAllButton);

    readConfig();
}

FilterSelectionDialog::~FilterSelectionDialog()
{
    writeConfig();
}

void FilterSelectionDialog::reject()
{
    qDeleteAll(originalFilters);
    QDialog::reject();
}

void FilterSelectionDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(300, 350));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myFilterSelectionDialogName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void FilterSelectionDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myFilterSelectionDialogName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

void FilterSelectionDialog::setFilters(const QList<MailFilter *> &filters)
{
    if (filters.isEmpty()) {
        mOkButton->setEnabled(false);
        return;
    }

    originalFilters = filters;
    filtersListWidget->clear();

    for (const MailFilter *filter : filters) {
        auto item = new QListWidgetItem(filter->name(), filtersListWidget);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Checked);
    }
}

QList<MailFilter *> FilterSelectionDialog::selectedFilters() const
{
    QList<MailFilter *> filters;

    const int filterCount = filtersListWidget->count();
    for (int i = 0; i < filterCount; ++i) {
        const QListWidgetItem *item = filtersListWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            filters << originalFilters[i];
        } else {
            delete originalFilters[i];
        }
    }

    return filters;
}

void FilterSelectionDialog::slotUnselectAllButton()
{
    const int filterCount = filtersListWidget->count();
    for (int i = 0; i < filterCount; ++i) {
        QListWidgetItem *const item = filtersListWidget->item(i);
        item->setCheckState(Qt::Unchecked);
    }
}

void FilterSelectionDialog::slotSelectAllButton()
{
    const int filterCount = filtersListWidget->count();
    for (int i = 0; i < filterCount; ++i) {
        QListWidgetItem *const item = filtersListWidget->item(i);
        item->setCheckState(Qt::Checked);
    }
}

#include "moc_filterselectiondialog.cpp"
