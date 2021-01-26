/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filteractionwidget.h"
#include "filter/filteractions/filteraction.h"
#include "filter/filteractions/filteractiondict.h"
#include "filter/filtermanager.h"
#include "filter/mailfilter.h"
#include "mailcommon_debug.h"

#include <KLocalizedString>
#include <QComboBox>
#include <QIcon>
#include <QPushButton>

#include <QGridLayout>
#include <QLabel>

using namespace MailCommon;

//=============================================================================
//
// class FilterActionWidget
//
//=============================================================================

class Q_DECL_HIDDEN FilterActionWidget::Private
{
public:
    Private(FilterActionWidget *qq)
        : q(qq)
    {
    }

    ~Private()
    {
        qDeleteAll(mActionList);
        mActionList.clear();
    }

    void setFilterAction(QWidget *widget = nullptr);

    void slotFilterTypeChanged(int index);
    void slotAddWidget();
    void slotRemoveWidget();

    FilterActionWidget *const q;
    QList<MailCommon::FilterAction *> mActionList;
    QComboBox *mComboBox = nullptr;
    QPushButton *mAdd = nullptr;
    QPushButton *mRemove = nullptr;

    QGridLayout *mLayout = nullptr;
};

void FilterActionWidget::Private::setFilterAction(QWidget *widget)
{
    if (mLayout->itemAtPosition(1, 2)) {
        delete mLayout->itemAtPosition(1, 2)->widget();
    }

    if (widget) {
        mLayout->addWidget(widget, 1, 2);
    } else {
        mLayout->addWidget(new QLabel(i18n("Please select an action."), q), 1, 2);
    }
}

void FilterActionWidget::Private::slotAddWidget()
{
    Q_EMIT q->addFilterWidget(q);
    Q_EMIT q->filterModified();
}

void FilterActionWidget::Private::slotRemoveWidget()
{
    Q_EMIT q->removeFilterWidget(q);
    Q_EMIT q->filterModified();
}

void FilterActionWidget::Private::slotFilterTypeChanged(int index)
{
    setFilterAction(index < mActionList.count() ? mActionList.at(index)->createParamWidget(q) : nullptr);
}

FilterActionWidget::FilterActionWidget(QWidget *parent)
    : QWidget(parent)
    , d(new Private(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    auto widget = new QWidget(this);
    mainLayout->addWidget(widget);

    d->mLayout = new QGridLayout(widget);
    d->mLayout->setContentsMargins(0, 0, 0, 0);

    d->mComboBox = new QComboBox(widget);
    d->mComboBox->setMinimumWidth(50);
    d->mComboBox->setEditable(false);
    Q_ASSERT(d->mComboBox);
    d->mLayout->addWidget(d->mComboBox, 1, 1);
    d->mAdd = new QPushButton(widget);
    d->mAdd->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    d->mAdd->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    d->mRemove = new QPushButton(widget);
    d->mRemove->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    d->mRemove->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    mainLayout->setSpacing(4);

    int index;
    QVector<FilterActionDesc *> list = MailCommon::FilterManager::filterActionDict()->list();
    QVector<FilterActionDesc *>::const_iterator it;
    QVector<FilterActionDesc *>::const_iterator end(list.constEnd());
    for (index = 0, it = list.constBegin(); it != end; ++it, ++index) {
        // create an instance:
        FilterAction *action = (*it)->create();

        // append to the list of actions:
        d->mActionList.append(action);

        // add (i18n-ized) name to combo box
        d->mComboBox->addItem((*it)->label, (*it)->name);

        // Register the FilterAction modification signal
        connect(action, &FilterAction::filterActionModified, this, &FilterActionWidget::filterModified);
    }

    // widget for the case where no action is selected.
    d->mComboBox->addItem(QStringLiteral(" "));
    d->mComboBox->setCurrentIndex(index);

    // don't show scroll bars.
    d->mComboBox->setMaxCount(d->mComboBox->count());

    // layout management:
    // o the combo box is not to be made larger than it's sizeHint(),
    //   the parameter widget should grow instead.
    // o the whole widget takes all space horizontally, but is fixed vertically.
    d->mComboBox->adjustSize();
    d->mComboBox->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    updateGeometry();

    // redirect focus to the filter action combo box
    setFocusProxy(d->mComboBox);

    // now connect the combo box and the widget stack
    connect(d->mComboBox, QOverload<int>::of(&QComboBox::activated), this, [this](int index) {
        d->slotFilterTypeChanged(index);
    });

    connect(d->mComboBox, QOverload<int>::of(&QComboBox::activated), this, &FilterActionWidget::filterModified);

    connect(d->mAdd, &QPushButton::clicked, this, [this]() {
        d->slotAddWidget();
    });
    connect(d->mRemove, &QPushButton::clicked, this, [this]() {
        d->slotRemoveWidget();
    });

    d->setFilterAction();
    d->mLayout->addWidget(d->mAdd, 1, 3);
    d->mLayout->addWidget(d->mRemove, 1, 4);
}

FilterActionWidget::~FilterActionWidget()
{
    delete d;
}

void FilterActionWidget::updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled)
{
    d->mAdd->setEnabled(addButtonEnabled);
    d->mRemove->setEnabled(removeButtonEnabled);
}

void FilterActionWidget::setAction(const FilterAction *action)
{
    bool found = false;
    const int count = d->mComboBox->count() - 1; // last entry is the empty one

    const QString name = action ? action->name() : QString();

    // find the index of typeOf(action) in mComboBox
    // and clear the other widgets on the way.
    for (int i = 0; i < count; ++i) {
        if (action && d->mComboBox->itemData(i) == name) {
            d->setFilterAction(d->mActionList.at(i)->createParamWidget(this));

            //...set the parameter widget to the settings
            // of aAction...
            action->setParamWidgetValue(d->mLayout->itemAtPosition(1, 2)->widget());

            //...and show the correct entry of
            // the combo box
            d->mComboBox->setCurrentIndex(i); // (mm) also raise the widget, but doesn't
            found = true;
        }
    }

    if (found) {
        return;
    }

    // not found, so set the empty widget
    d->setFilterAction();

    d->mComboBox->setCurrentIndex(count); // last item
}

FilterAction *FilterActionWidget::action() const
{
    // look up the action description via the label
    // returned by KComboBox::currentText()...
    FilterActionDesc *description = MailCommon::FilterManager::filterActionDict()->value(d->mComboBox->itemData(d->mComboBox->currentIndex()).toString());

    if (description) {
        // ...create an instance...
        FilterAction *action = description->create();
        if (action) {
            // ...and apply the setting of the parameter widget.
            action->applyParamWidgetValue(d->mLayout->itemAtPosition(1, 2)->widget());
            return action;
        }
    }

    return nullptr;
}

//=============================================================================
//
// class FilterActionWidgetLister (the filter action editor)
//
//=============================================================================

class Q_DECL_HIDDEN FilterActionWidgetLister::Private
{
public:
    Private(FilterActionWidgetLister *qq)
        : q(qq)
    {
    }

    void regenerateActionListFromWidgets();

    FilterActionWidgetLister *const q;
    QVector<MailCommon::FilterAction *> *mActionList = nullptr;
};

void FilterActionWidgetLister::Private::regenerateActionListFromWidgets()
{
    if (!mActionList) {
        return;
    }

    mActionList->clear();

    const auto lst = q->widgets();
    for (const QWidget *widget : lst) {
        FilterAction *action = qobject_cast<const FilterActionWidget *>(widget)->action();
        if (action) {
            mActionList->append(action);
        }
    }
    q->updateAddRemoveButton();
}

FilterActionWidgetLister::FilterActionWidgetLister(QWidget *parent)
    : KWidgetLister(false, 1, MailFilter::filterActionsMaximumSize(), parent)
    , d(new Private(this))
{
}

FilterActionWidgetLister::~FilterActionWidgetLister()
{
    delete d;
}

void FilterActionWidgetLister::setActionList(QVector<FilterAction *> *list)
{
    Q_ASSERT(list);
    if (d->mActionList && d->mActionList != list) {
        d->regenerateActionListFromWidgets();
    }

    d->mActionList = list;

    static_cast<QWidget *>(parent())->setEnabled(true);

    if (!widgets().isEmpty()) { // move this below next 'if'?
        widgets().constFirst()->blockSignals(true);
    }

    if (list->isEmpty()) {
        slotClear();
        connectWidget(widgets().constFirst(), nullptr);
        widgets().constFirst()->blockSignals(false);
        return;
    }

    int superfluousItems = (int)d->mActionList->count() - widgetsMaximum();
    if (superfluousItems > 0) {
        qCDebug(MAILCOMMON_LOG) << "FilterActionWidgetLister: Clipping action list to" << widgetsMaximum() << "items!";

        for (; superfluousItems; superfluousItems--) {
            d->mActionList->removeLast();
        }
    }

    // set the right number of widgets
    setNumberOfShownWidgetsTo(d->mActionList->count());

    // load the actions into the widgets
    QList<QWidget *> widgetList = widgets();
    QVector<FilterAction *>::const_iterator aEnd(d->mActionList->constEnd());
    QList<QWidget *>::ConstIterator wIt = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (QVector<FilterAction *>::const_iterator aIt = d->mActionList->constBegin(); (aIt != aEnd && wIt != wEnd); ++aIt, ++wIt) {
        connectWidget((*wIt), (*aIt));
    }
    widgets().constFirst()->blockSignals(false);
    updateAddRemoveButton();
}

void FilterActionWidgetLister::connectWidget(QWidget *widget, FilterAction *filterAction)
{
    auto w = qobject_cast<FilterActionWidget *>(widget);
    if (filterAction) {
        w->setAction(filterAction);
    }
    connect(w, &FilterActionWidget::filterModified, this, &FilterActionWidgetLister::filterModified, Qt::UniqueConnection);
    reconnectWidget(w);
}

void FilterActionWidgetLister::slotAddWidget(QWidget *w)
{
    addWidgetAfterThisWidget(w);
    updateAddRemoveButton();
}

void FilterActionWidgetLister::slotRemoveWidget(QWidget *w)
{
    removeWidget(w);
    updateAddRemoveButton();
}

void FilterActionWidgetLister::updateAddRemoveButton()
{
    QList<QWidget *> widgetList = widgets();
    const int numberOfWidget(widgetList.count());
    bool addButtonEnabled = false;
    bool removeButtonEnabled = false;
    if (numberOfWidget <= widgetsMinimum()) {
        addButtonEnabled = true;
        removeButtonEnabled = false;
    } else if (numberOfWidget >= widgetsMaximum()) {
        addButtonEnabled = false;
        removeButtonEnabled = true;
    } else {
        addButtonEnabled = true;
        removeButtonEnabled = true;
    }
    QList<QWidget *>::ConstIterator wIt = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (; wIt != wEnd; ++wIt) {
        auto w = qobject_cast<FilterActionWidget *>(*wIt);
        w->updateAddRemoveButton(addButtonEnabled, removeButtonEnabled);
    }
}

void FilterActionWidgetLister::updateActionList()
{
    d->regenerateActionListFromWidgets();
}

void FilterActionWidgetLister::reset()
{
    if (d->mActionList) {
        d->regenerateActionListFromWidgets();
    }

    d->mActionList = nullptr;
    slotClear();

    static_cast<QWidget *>(parent())->setEnabled(false);
}

void FilterActionWidgetLister::reconnectWidget(FilterActionWidget *w)
{
    connect(w, &FilterActionWidget::addFilterWidget, this, &FilterActionWidgetLister::slotAddWidget, Qt::UniqueConnection);

    connect(w, &FilterActionWidget::removeFilterWidget, this, &FilterActionWidgetLister::slotRemoveWidget, Qt::UniqueConnection);
}

QWidget *FilterActionWidgetLister::createWidget(QWidget *parent)
{
    auto w = new FilterActionWidget(parent);
    reconnectWidget(w);
    return w;
}

void FilterActionWidgetLister::clearWidget(QWidget *widget)
{
    if (widget) {
        auto w = static_cast<FilterActionWidget *>(widget);
        w->setAction(nullptr);
        w->disconnect(this);
        reconnectWidget(w);
        updateAddRemoveButton();
    }
}

#include "moc_filteractionwidget.cpp"
