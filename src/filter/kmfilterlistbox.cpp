/*
  SPDX-FileCopyrightText: 2014-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kmfilterlistbox.h"
#include "filteractions/filteractiondict.h"
#include "filtermanager.h"
#include "invalidfilters/invalidfilterdialog.h"
#include "invalidfilters/invalidfilterinfo.h"
#include "mailcommon_debug.h"
#include "mailfilter.h"
#include <KListWidgetSearchLine>
#include <KLocalizedString>
#include <KMessageBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QKeyEvent>
#include <QListWidget>
#include <QPointer>
#include <QPushButton>
#include <QShortcut>
#include <QVBoxLayout>

//=============================================================================
//
// class KMFilterListBox (the filter list manipulator)
//
//=============================================================================
using namespace MailCommon;
KMFilterListBox::KMFilterListBox(const QString &title, QWidget *parent)
    : QGroupBox(title, parent)
{
    auto layout = new QVBoxLayout(this);

    //----------- the list box
    mListWidget = new QListWidget(this);
    mListWidget->setMinimumWidth(150);
    mListWidget->setWhatsThis(
        i18n("<qt><p>This is the list of defined filters. "
             "They are processed top-to-bottom.</p>"
             "<p>Click on any filter to edit it "
             "using the controls in the right-hand half "
             "of the dialog.</p></qt>"));
    mListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    mListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(mListWidget->model(), &QAbstractItemModel::rowsMoved, this, &KMFilterListBox::slotRowsMoved);

    mSearchListWidget = new KListWidgetSearchLine(this, mListWidget);
    mSearchListWidget->setPlaceholderText(i18nc("@info Displayed grayed-out inside the textbox, verb to search", "Search"));
    mSearchListWidget->installEventFilter(this);
    layout->addWidget(mSearchListWidget);
    layout->addWidget(mListWidget);

    //----------- the first row of buttons
    auto hb = new QWidget(this);
    auto hbHBoxLayout = new QHBoxLayout(hb);
    hbHBoxLayout->setContentsMargins({});
    hbHBoxLayout->setSpacing(4);

    mBtnTop = new QPushButton(QString(), hb);
    hbHBoxLayout->addWidget(mBtnTop);
    mBtnTop->setIcon(QIcon::fromTheme(QStringLiteral("go-top")));
    mBtnTop->setMinimumSize(mBtnTop->sizeHint() * 1.2);

    mBtnUp = new QPushButton(QString(), hb);
    hbHBoxLayout->addWidget(mBtnUp);
    mBtnUp->setAutoRepeat(true);
    mBtnUp->setIcon(QIcon::fromTheme(QStringLiteral("go-up")));
    mBtnUp->setMinimumSize(mBtnUp->sizeHint() * 1.2);
    mBtnDown = new QPushButton(QString(), hb);
    hbHBoxLayout->addWidget(mBtnDown);
    mBtnDown->setAutoRepeat(true);
    mBtnDown->setIcon(QIcon::fromTheme(QStringLiteral("go-down")));
    mBtnDown->setMinimumSize(mBtnDown->sizeHint() * 1.2);

    mBtnBottom = new QPushButton(QString(), hb);
    hbHBoxLayout->addWidget(mBtnBottom);
    mBtnBottom->setIcon(QIcon::fromTheme(QStringLiteral("go-bottom")));
    mBtnBottom->setMinimumSize(mBtnBottom->sizeHint() * 1.2);

    mBtnUp->setToolTip(i18nc("Move selected filter up.", "Up"));
    mBtnDown->setToolTip(i18nc("Move selected filter down.", "Down"));
    mBtnTop->setToolTip(i18nc("Move selected filter to the top.", "Top"));
    mBtnBottom->setToolTip(i18nc("Move selected filter to the bottom.", "Bottom"));
    mBtnUp->setWhatsThis(
        i18n("<qt><p>Click this button to move the currently-"
             "selected filter <em>up</em> one in the list above.</p>"
             "<p>This is useful since the order of the filters in the list "
             "determines the order in which they are tried on messages: "
             "The topmost filter gets tried first.</p>"
             "<p>If you have clicked this button accidentally, you can undo this "
             "by clicking on the <em>Down</em> button.</p></qt>"));
    mBtnDown->setWhatsThis(
        i18n("<qt><p>Click this button to move the currently-"
             "selected filter <em>down</em> one in the list above.</p>"
             "<p>This is useful since the order of the filters in the list "
             "determines the order in which they are tried on messages: "
             "The topmost filter gets tried first.</p>"
             "<p>If you have clicked this button accidentally, you can undo this "
             "by clicking on the <em>Up</em> button.</p></qt>"));
    mBtnBottom->setWhatsThis(
        i18n("<qt><p>Click this button to move the currently-"
             "selected filter to bottom of list.</p>"
             "<p>This is useful since the order of the filters in the list "
             "determines the order in which they are tried on messages: "
             "The topmost filter gets tried first.</p></qt>"));
    mBtnTop->setWhatsThis(
        i18n("<qt><p>Click this button to move the currently-"
             "selected filter to top of list.</p>"
             "<p>This is useful since the order of the filters in the list "
             "determines the order in which they are tried on messages: "
             "The topmost filter gets tried first.</p></qt>"));

    layout->addWidget(hb);

    //----------- the second row of buttons
    hb = new QWidget(this);
    hbHBoxLayout = new QHBoxLayout(hb);
    hbHBoxLayout->setContentsMargins({});
    hbHBoxLayout->setSpacing(4);
    mBtnNew = new QPushButton(hb);
    hbHBoxLayout->addWidget(mBtnNew);
    mBtnNew->setIcon(QIcon::fromTheme(QStringLiteral("document-new")));
    mBtnNew->setMinimumSize(mBtnNew->sizeHint() * 1.2);
    mBtnCopy = new QPushButton(hb);
    hbHBoxLayout->addWidget(mBtnCopy);
    mBtnCopy->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));
    mBtnCopy->setMinimumSize(mBtnCopy->sizeHint() * 1.2);
    mBtnDelete = new QPushButton(hb);
    hbHBoxLayout->addWidget(mBtnDelete);
    mBtnDelete->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete")));
    mBtnDelete->setMinimumSize(mBtnDelete->sizeHint() * 1.2);
    mBtnRename = new QPushButton(hb);
    mBtnRename->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename")));
    mBtnRename->setMinimumSize(mBtnDelete->sizeHint() * 1.2);

    hbHBoxLayout->addWidget(mBtnRename);
    mBtnNew->setToolTip(i18nc("@action:button in filter list manipulator", "New"));
    mBtnCopy->setToolTip(i18nc("@info:tooltip", "Copy"));
    mBtnDelete->setToolTip(i18nc("@info:tooltip", "Delete"));
    mBtnRename->setToolTip(i18nc("@info:tooltip", "Rename"));
    mBtnNew->setWhatsThis(
        i18n("<qt><p>Click this button to create a new filter.</p>"
             "<p>The filter will be inserted just before the currently-"
             "selected one, but you can always change that "
             "later on.</p>"
             "<p>If you have clicked this button accidentally, you can undo this "
             "by clicking on the <em>Delete</em> button.</p></qt>"));
    mBtnCopy->setWhatsThis(
        i18n("<qt><p>Click this button to copy a filter.</p>"
             "<p>If you have clicked this button accidentally, you can undo this "
             "by clicking on the <em>Delete</em> button.</p></qt>"));
    mBtnDelete->setWhatsThis(
        i18n("<qt><p>Click this button to <em>delete</em> the currently-"
             "selected filter from the list above.</p>"
             "<p>There is no way to get the filter back once "
             "it is deleted, but you can always leave the "
             "dialog by clicking <em>Cancel</em> to discard the "
             "changes made.</p></qt>"));
    mBtnRename->setWhatsThis(
        i18n("<qt><p>Click this button to rename the currently-selected filter.</p>"
             "<p>Filters are named automatically, as long as they start with "
             "\"&lt;\".</p>"
             "<p>If you have renamed a filter accidentally and want automatic "
             "naming back, click this button and select <em>Clear</em> followed "
             "by <em>OK</em> in the appearing dialog.</p></qt>"));

    layout->addWidget(hb);

    auto shortcut = new QShortcut(this);
    shortcut->setKey(Qt::Key_Delete);
    connect(shortcut, &QShortcut::activated, this, &KMFilterListBox::slotDelete);

    //----------- now connect everything
    connect(mListWidget, &QListWidget::currentRowChanged, this, &KMFilterListBox::slotSelected);
    connect(mListWidget, &QListWidget::itemDoubleClicked, this, &KMFilterListBox::slotRename);
    connect(mListWidget, &QListWidget::itemChanged, this, &KMFilterListBox::slotFilterEnabledChanged);

    connect(mListWidget, &QListWidget::itemSelectionChanged, this, &KMFilterListBox::slotSelectionChanged);

    connect(mBtnUp, &QPushButton::clicked, this, &KMFilterListBox::slotUp);
    connect(mBtnDown, &QPushButton::clicked, this, &KMFilterListBox::slotDown);
    connect(mBtnTop, &QPushButton::clicked, this, &KMFilterListBox::slotTop);
    connect(mBtnBottom, &QPushButton::clicked, this, &KMFilterListBox::slotBottom);

    connect(mBtnNew, &QPushButton::clicked, this, &KMFilterListBox::slotNew);
    connect(mBtnCopy, &QPushButton::clicked, this, &KMFilterListBox::slotCopy);
    connect(mBtnDelete, &QPushButton::clicked, this, &KMFilterListBox::slotDelete);
    connect(mBtnRename, &QPushButton::clicked, this, &KMFilterListBox::slotRename);

    // the dialog should call loadFilterList()
    // when all signals are connected.
    enableControls();
}

KMFilterListBox::~KMFilterListBox() = default;

bool KMFilterListBox::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && obj == mSearchListWidget) {
        auto key = static_cast<QKeyEvent *>(event);
        if ((key->key() == Qt::Key_Enter) || (key->key() == Qt::Key_Return)) {
            event->accept();
            return true;
        }
    }
    return QGroupBox::eventFilter(obj, event);
}

bool KMFilterListBox::itemIsValid(QListWidgetItem *item) const
{
    if (!item) {
        qCDebug(MAILCOMMON_LOG) << "Called while no filter is selected, ignoring.";
        return false;
    }
    if (item->isHidden()) {
        return false;
    }
    return true;
}

void KMFilterListBox::slotFilterEnabledChanged(QListWidgetItem *item)
{
    if (!item) {
        qCDebug(MAILCOMMON_LOG) << "Called while no filter is selected, ignoring.";
        return;
    }
    auto itemFilter = static_cast<QListWidgetFilterItem *>(item);
    MailCommon::MailFilter *filter = itemFilter->filter();
    filter->setEnabled((item->checkState() == Qt::Checked));
    Q_EMIT filterUpdated(filter);
}

void KMFilterListBox::slotRowsMoved(const QModelIndex &, int sourcestart, int sourceEnd, const QModelIndex &, int destinationRow)
{
    Q_UNUSED(sourceEnd)
    Q_UNUSED(sourcestart)
    Q_UNUSED(destinationRow)
    enableControls();

    Q_EMIT filterOrderAltered();
}

void KMFilterListBox::createFilter(const QByteArray &field, const QString &value)
{
    SearchRule::Ptr newRule = SearchRule::createInstance(field, SearchRule::FuncContains, value);

    auto newFilter = new MailFilter();
    newFilter->pattern()->append(newRule);
    newFilter->pattern()->setName(QStringLiteral("<%1>: %2").arg(QString::fromLatin1(field), value));

    FilterActionDesc *desc = MailCommon::FilterManager::filterActionDict()->value(QStringLiteral("transfer"));
    if (desc) {
        newFilter->actions()->append(desc->create());
    }

    insertFilter(newFilter);
    enableControls();
}

void KMFilterListBox::slotUpdateFilterName()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (!item) {
        qCDebug(MAILCOMMON_LOG) << "Called while no filter is selected, ignoring.";
        return;
    }
    auto itemFilter = static_cast<QListWidgetFilterItem *>(item);
    MailCommon::MailFilter *filter = itemFilter->filter();

    SearchPattern *p = filter->pattern();
    if (!p) {
        return;
    }

    QString shouldBeName = p->name();
    QString displayedName = itemFilter->text();

    if (shouldBeName.trimmed().isEmpty()) {
        filter->setAutoNaming(true);
    }

    if (filter->isAutoNaming()) {
        // auto-naming of patterns
        if (!p->isEmpty() && p->first() && !p->first()->field().trimmed().isEmpty()) {
            shouldBeName = QStringLiteral("<%1>: %2").arg(QString::fromLatin1(p->first()->field()), p->first()->contents());
        } else {
            shouldBeName = QLatin1Char('<') + i18n("unnamed") + QLatin1Char('>');
        }
        p->setName(shouldBeName);
    }

    if (displayedName == shouldBeName) {
        return;
    }

    filter->setToolbarName(shouldBeName);

    mListWidget->blockSignals(true);
    itemFilter->setText(shouldBeName);
    mListWidget->blockSignals(false);
}

void KMFilterListBox::slotAccepted()
{
    applyFilterChanged(true);
}

void KMFilterListBox::slotApplied()
{
    applyFilterChanged(false);
}

void KMFilterListBox::applyFilterChanged(bool closeAfterSaving)
{
    if (mListWidget->currentItem()) {
        Q_EMIT applyWidgets();
        slotSelected(mListWidget->currentRow());
    }

    // by now all edit widgets should have written back
    // their widget's data into our filter list.

    bool wasCanceled = false;
    const QList<MailFilter *> newFilters = filtersForSaving(closeAfterSaving, wasCanceled);
    if (!wasCanceled) {
        MailCommon::FilterManager::instance()->setFilters(newFilters);
    }
}

QList<MailFilter *> KMFilterListBox::filtersForSaving(bool closeAfterSaving, bool &wasCanceled) const
{
    Q_EMIT const_cast<KMFilterListBox *>(this)->applyWidgets(); // signals aren't const
    QList<MailFilter *> filters;
    QStringList emptyFilters;
    QList<MailCommon::InvalidFilterInfo> listInvalidFilters;
    const int numberOfFilter(mListWidget->count());
    for (int i = 0; i < numberOfFilter; ++i) {
        auto itemFilter = static_cast<QListWidgetFilterItem *>(mListWidget->item(i));
        auto f = new MailFilter(*itemFilter->filter()); // deep copy

        const QString information = f->purify();
        if (!f->isEmpty() && information.isEmpty()) {
            // the filter is valid:
            filters.append(f);
        } else {
            // the filter is invalid:
            emptyFilters << f->name();
            listInvalidFilters.append(MailCommon::InvalidFilterInfo(f->name(), information));
            delete f;
        }
    }

    // report on invalid filters:
    if (!emptyFilters.empty()) {
        QPointer<MailCommon::InvalidFilterDialog> dlg = new MailCommon::InvalidFilterDialog(nullptr);
        dlg->setInvalidFilters(listInvalidFilters);
        if (!dlg->exec()) {
            if (closeAfterSaving) {
                Q_EMIT abortClosing();
            }
            wasCanceled = true;
        }
        delete dlg;
    }
    return filters;
}

void KMFilterListBox::slotSelectionChanged()
{
    if (mListWidget->selectedItems().count() > 1) {
        Q_EMIT resetWidgets();
    }
    enableControls();
}

void KMFilterListBox::slotSelected(int aIdx)
{
    if (aIdx >= 0 && aIdx < mListWidget->count()) {
        auto itemFilter = static_cast<QListWidgetFilterItem *>(mListWidget->item(aIdx));
        MailFilter *f = itemFilter->filter();

        if (f) {
            Q_EMIT filterSelected(f);
        } else {
            Q_EMIT resetWidgets();
        }
    } else {
        Q_EMIT resetWidgets();
    }
    enableControls();
}

void KMFilterListBox::slotNew()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (item && item->isHidden()) {
        return;
    }
    // just insert a new filter.
    insertFilter(new MailFilter());
    enableControls();
}

void KMFilterListBox::slotCopy()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (!itemIsValid(item)) {
        return;
    }

    // make sure that all changes are written to the filter before we copy it
    Q_EMIT applyWidgets();
    auto itemFilter = static_cast<QListWidgetFilterItem *>(item);

    MailFilter *filter = itemFilter->filter();

    // enableControls should make sure this method is
    // never called when no filter is selected.
    Q_ASSERT(filter);

    // inserts a copy of the current filter.
    auto copyFilter = new MailFilter(*filter);
    copyFilter->generateRandomIdentifier();
    copyFilter->setShortcut(QKeySequence());

    insertFilter(copyFilter);
    enableControls();
}

void KMFilterListBox::slotDelete()
{
    QListWidgetItem *itemFirst = mListWidget->currentItem();
    if (!itemIsValid(itemFirst)) {
        return;
    }
    const bool uniqFilterSelected = (mListWidget->selectedItems().count() == 1);

    auto itemFilter = static_cast<QListWidgetFilterItem *>(itemFirst);
    MailCommon::MailFilter *filter = itemFilter->filter();
    const QString question =
        uniqFilterSelected ? i18n("Do you want to remove the filter \"%1\"?", filter->pattern()->name()) : i18n("Do you want to remove selected filters?");
    const QString dialogTitle = uniqFilterSelected ? i18nc("@title:window", "Remove Filter") : i18nc("@title:window", "Remove Filters");
    const int answer = KMessageBox::questionTwoActions(this, question, dialogTitle, KStandardGuiItem::remove(), KStandardGuiItem::cancel());
    if (answer == KMessageBox::ButtonCode::SecondaryAction) {
        return;
    }

    const int oIdxSelItem = mListWidget->currentRow();
    QList<MailCommon::MailFilter *> lst;

    Q_EMIT resetWidgets();

    const QList<QListWidgetItem *> lstItems = mListWidget->selectedItems();
    for (QListWidgetItem *item : lstItems) {
        auto itemFilter = static_cast<QListWidgetFilterItem *>(item);

        MailCommon::MailFilter *filter = itemFilter->filter();
        lst << filter;

        // remove the filter from both the listbox
        QListWidgetItem *item2 = mListWidget->takeItem(mListWidget->row(item));
        delete item2;
    }
    const int count = mListWidget->count();
    // and set the new current item.
    if (count > oIdxSelItem) {
        // oIdxItem is still a valid index
        mListWidget->setCurrentRow(oIdxSelItem);
    } else if (count) {
        // oIdxSelIdx is no longer valid, but the
        // list box isn't empty
        mListWidget->setCurrentRow(count - 1);
    }

    // work around a problem when deleting the first item in a QListWidget:
    // after takeItem, slotSelectionChanged is emitted with 1, but the row 0
    // remains selected and another selectCurrentRow(0) does not trigger the
    // selectionChanged signal
    // (qt-copy as of 2006-12-22 / gungl)
    if (oIdxSelItem == 0) {
        slotSelected(0);
    }
    enableControls();

    Q_EMIT filterRemoved(lst);
}

void KMFilterListBox::slotTop()
{
    QList<QListWidgetItem *> listWidgetItem = selectedFilter();
    if (listWidgetItem.isEmpty()) {
        return;
    }

    const int numberOfItem(listWidgetItem.count());
    if ((numberOfItem == 1) && (mListWidget->currentRow() == 0)) {
        qCDebug(MAILCOMMON_LOG) << "Called while the _topmost_ filter is selected, ignoring.";
        return;
    }

    QListWidgetItem *item = nullptr;
    bool wasMoved = false;
    for (int i = 0; i < numberOfItem; ++i) {
        const int posItem = mListWidget->row(listWidgetItem.at(i));
        if (posItem == i) {
            continue;
        }
        item = mListWidget->takeItem(mListWidget->row(listWidgetItem.at(i)));
        mListWidget->insertItem(i, item);
        wasMoved = true;
    }

    if (wasMoved) {
        enableControls();
        Q_EMIT filterOrderAltered();
    }
}

QList<QListWidgetItem *> KMFilterListBox::selectedFilter()
{
    QList<QListWidgetItem *> listWidgetItem;
    const int numberOfFilters = mListWidget->count();
    for (int i = 0; i < numberOfFilters; ++i) {
        if (mListWidget->item(i)->isSelected() && !mListWidget->item(i)->isHidden()) {
            listWidgetItem << mListWidget->item(i);
        }
    }
    return listWidgetItem;
}

QStringList KMFilterListBox::selectedFilterId(SearchRule::RequiredPart &requiredPart, const QString &resource) const
{
    QStringList listFilterId;
    requiredPart = SearchRule::Envelope;
    const int numberOfFilters = mListWidget->count();
    for (int i = 0; i < numberOfFilters; ++i) {
        if (mListWidget->item(i)->isSelected() && !mListWidget->item(i)->isHidden()) {
            MailFilter *filter = static_cast<QListWidgetFilterItem *>(mListWidget->item(i))->filter();
            if (!filter->isEmpty()) {
                const QString id = filter->identifier();
                listFilterId << id;
                requiredPart = qMax(requiredPart, static_cast<QListWidgetFilterItem *>(mListWidget->item(i))->filter()->requiredPart(resource));
            }
        }
    }
    return listFilterId;
}

void KMFilterListBox::slotBottom()
{
    const QList<QListWidgetItem *> listWidgetItem = selectedFilter();
    if (listWidgetItem.isEmpty()) {
        return;
    }

    const int numberOfElement(mListWidget->count());
    const int numberOfItem(listWidgetItem.count());
    if ((numberOfItem == 1) && (mListWidget->currentRow() == numberOfElement - 1)) {
        qCDebug(MAILCOMMON_LOG) << "Called while the _last_ filter is selected, ignoring.";
        return;
    }

    QListWidgetItem *item = nullptr;
    int j = 0;
    bool wasMoved = false;
    for (int i = numberOfItem - 1; i >= 0; --i, j++) {
        const int posItem = mListWidget->row(listWidgetItem.at(i));
        if (posItem == (numberOfElement - 1 - j)) {
            continue;
        }
        item = mListWidget->takeItem(mListWidget->row(listWidgetItem.at(i)));
        mListWidget->insertItem(numberOfElement - j, item);
        wasMoved = true;
    }

    if (wasMoved) {
        enableControls();
        Q_EMIT filterOrderAltered();
    }
}

void KMFilterListBox::slotUp()
{
    const QList<QListWidgetItem *> listWidgetItem = selectedFilter();
    if (listWidgetItem.isEmpty()) {
        return;
    }

    const int numberOfItem(listWidgetItem.count());
    if ((numberOfItem == 1) && (mListWidget->currentRow() == 0)) {
        qCDebug(MAILCOMMON_LOG) << "Called while the _topmost_ filter is selected, ignoring.";
        return;
    }
    bool wasMoved = false;

    for (int i = 0; i < numberOfItem; ++i) {
        const int posItem = mListWidget->row(listWidgetItem.at(i));
        if (posItem == i) {
            continue;
        }
        swapNeighbouringFilters(posItem, posItem - 1);
        wasMoved = true;
    }
    if (wasMoved) {
        enableControls();
        Q_EMIT filterOrderAltered();
    }
}

void KMFilterListBox::slotDown()
{
    const QList<QListWidgetItem *> listWidgetItem = selectedFilter();
    if (listWidgetItem.isEmpty()) {
        return;
    }

    const int numberOfElement(mListWidget->count());
    const int numberOfItem(listWidgetItem.count());
    if ((numberOfItem == 1) && (mListWidget->currentRow() == numberOfElement - 1)) {
        qCDebug(MAILCOMMON_LOG) << "Called while the _last_ filter is selected, ignoring.";
        return;
    }

    int j = 0;
    bool wasMoved = false;
    for (int i = numberOfItem - 1; i >= 0; --i, j++) {
        const int posItem = mListWidget->row(listWidgetItem.at(i));
        if (posItem == (numberOfElement - 1 - j)) {
            continue;
        }
        swapNeighbouringFilters(posItem, posItem + 1);
        wasMoved = true;
    }

    if (wasMoved) {
        enableControls();
        Q_EMIT filterOrderAltered();
    }
}

void KMFilterListBox::slotRename()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (!itemIsValid(item)) {
        return;
    }
    auto itemFilter = static_cast<QListWidgetFilterItem *>(item);

    bool okPressed = false;
    MailFilter *filter = itemFilter->filter();

    // enableControls should make sure this method is
    // never called when no filter is selected.
    Q_ASSERT(filter);

    // allow empty names - those will turn auto-naming on again
    QString newName = QInputDialog::getText(window(),
                                            i18n("Rename Filter"),
                                            i18n("Rename filter \"%1\" to:\n(leave the field empty for automatic naming)", filter->pattern()->name()), /*label*/
                                            QLineEdit::Normal,
                                            filter->pattern()->name(), /* initial value */
                                            &okPressed);

    if (!okPressed) {
        return;
    }

    if (newName.isEmpty()) {
        // bait for slotUpdateFilterName to
        // use automatic naming again.
        filter->pattern()->setName(QStringLiteral("<>"));
        filter->setAutoNaming(true);
    } else {
        filter->pattern()->setName(newName);
        filter->setAutoNaming(false);
    }

    slotUpdateFilterName();

    Q_EMIT filterUpdated(filter);
}

void KMFilterListBox::enableControls()
{
    const int currentIndex = mListWidget->currentRow();
    const bool theFirst = (currentIndex == 0);
    const int numberOfElement(mListWidget->count());
    const bool theLast = (currentIndex >= numberOfElement - 1);
    const bool aFilterIsSelected = (currentIndex >= 0);

    const int numberOfSelectedItem(mListWidget->selectedItems().count());
    const bool uniqFilterSelected = (numberOfSelectedItem == 1);
    const bool allItemSelected = (numberOfSelectedItem == numberOfElement);

    mBtnUp->setEnabled(aFilterIsSelected && ((uniqFilterSelected && !theFirst) || (!uniqFilterSelected)) && !allItemSelected);
    mBtnDown->setEnabled(aFilterIsSelected && ((uniqFilterSelected && !theLast) || (!uniqFilterSelected)) && !allItemSelected);

    mBtnCopy->setEnabled(aFilterIsSelected && uniqFilterSelected);
    mBtnDelete->setEnabled(aFilterIsSelected);
    mBtnRename->setEnabled(aFilterIsSelected && uniqFilterSelected);

    mBtnTop->setEnabled(aFilterIsSelected && ((uniqFilterSelected && !theFirst) || (!uniqFilterSelected)) && !allItemSelected);

    mBtnBottom->setEnabled(aFilterIsSelected && ((uniqFilterSelected && !theLast) || (!uniqFilterSelected)) && !allItemSelected);
    if (aFilterIsSelected) {
        mListWidget->scrollToItem(mListWidget->currentItem());
    }
}

void KMFilterListBox::loadFilterList(bool createDummyFilter)
{
    Q_ASSERT(mListWidget);
    setEnabled(false);
    Q_EMIT resetWidgets();
    // we don't want the insertion to
    // cause flicker in the edit widgets.
    blockSignals(true);

    // clear both lists
    mListWidget->clear();

    const QList<MailFilter *> filters = MailCommon::FilterManager::instance()->filters();
    for (MailFilter *filter : filters) {
        auto item = new QListWidgetFilterItem(filter->pattern()->name(), mListWidget);
        item->setFilter(new MailFilter(*filter));
        mListWidget->addItem(item);
    }

    blockSignals(false);
    setEnabled(true);

    // create an empty filter when there's none, to avoid a completely
    // disabled dialog (usability tests indicated that the new-filter
    // button is too hard to find that way):
    const int numberOfItem(mListWidget->count());
    if (numberOfItem == 0) {
        if (createDummyFilter) {
            slotNew();
        }
    } else {
        mListWidget->setCurrentRow(0);
    }

    enableControls();
}

void KMFilterListBox::insertFilter(MailFilter *aFilter)
{
    // must be really a filter...
    Q_ASSERT(aFilter);
    const int currentIndex = mListWidget->currentRow();
    // if mIdxSelItem < 0, QListBox::insertItem will append.
    auto item = new QListWidgetFilterItem(aFilter->pattern()->name());
    item->setFilter(aFilter);
    mListWidget->insertItem(currentIndex, item);
    mListWidget->clearSelection();
    if (currentIndex < 0) {
        mListWidget->setCurrentRow(mListWidget->count() - 1);
    } else {
        // insert just before selected
        mListWidget->setCurrentRow(currentIndex);
    }

    Q_EMIT filterCreated();
    Q_EMIT filterOrderAltered();
}

void KMFilterListBox::appendFilter(MailFilter *aFilter)
{
    auto item = new QListWidgetFilterItem(aFilter->pattern()->name(), mListWidget);

    item->setFilter(aFilter);
    mListWidget->addItem(item);

    Q_EMIT filterCreated();
}

void KMFilterListBox::swapNeighbouringFilters(int untouchedOne, int movedOne)
{
    // must be neighbours...
    Q_ASSERT(untouchedOne - movedOne == 1 || movedOne - untouchedOne == 1);

    // untouchedOne is at idx. to move it down(up),
    // remove item at idx+(-)1 w/o deleting it.
    QListWidgetItem *item = mListWidget->takeItem(movedOne);
    // now selected item is at idx(idx-1), so
    // insert the other item at idx, ie. above(below).
    mListWidget->insertItem(untouchedOne, item);
}

QListWidgetFilterItem::QListWidgetFilterItem(const QString &text, QListWidget *parent)
    : QListWidgetItem(text, parent)
{
}

QListWidgetFilterItem::~QListWidgetFilterItem()
{
    delete mFilter;
}

void QListWidgetFilterItem::setFilter(MailCommon::MailFilter *filter)
{
    mFilter = filter;
    setCheckState(filter->isEnabled() ? Qt::Checked : Qt::Unchecked);
}

MailCommon::MailFilter *QListWidgetFilterItem::filter() const
{
    return mFilter;
}

#include "moc_kmfilterlistbox.cpp"
