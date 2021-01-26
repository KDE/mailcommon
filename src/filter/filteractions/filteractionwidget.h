/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>
  SPDX-FileCopyrightText: Marc Mutz <mutz@kde.org>

  based upon work by Stefan Taferner <taferner@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FILTERACTIONWIDGET_H
#define MAILCOMMON_FILTERACTIONWIDGET_H

#include "mailcommon_export.h"

#include <Libkdepim/KWidgetLister>
#include <QVector>
namespace MailCommon
{
class FilterAction;
class FilterActionWidget;

/**
 * @short A widget to edit a single MailCommon::FilterAction.
 *
 * This widgets allows to edit a single MailCommon::FilterAction (in fact
 * any derived class that is registered in
 * KMFilterActionDict). It consists of a combo box which allows to
 * select the type of actions this widget should act upon.
 *
 * You can load a MailCommon::FilterAction into this widget with setAction,
 * and retrieve the result of user action with action.
 * The widget will copy it's setting into the corresponding
 * parameter widget. For that, it internally creates an instance of
 * every MailCommon::FilterAction in KMFilterActionDict and asks each
 * one to create a parameter widget.
 *
 * @author Marc Mutz <mutz@kde.org>
 * @see MailCommon::FilterAction MailCommon::MailFilter FilterActionWidgetLister
 */
class FilterActionWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Creates a filter action widget with no type selected.
     *
     * @param parent The parent widget.
     */
    explicit FilterActionWidget(QWidget *parent = nullptr);

    /**
     * Destroys the filter action widget.
     */
    ~FilterActionWidget();

    /**
     * Sets the filter action.
     *
     * The action's type is determined and the corresponding widget
     * it loaded with @p action's parameters and then raised.
     *
     * If @p action is @c 0, the widget is cleared.
     * @note The widget takes ownership of the passed action.
     */
    void setAction(const MailCommon::FilterAction *action);

    /**
     * Returns the filter action.
     *
     * This method is necessary because the type of actions can
     * change during editing. Therefore the widget always creates a new
     * action object from the data in the combo box and returns that.
     */
    Q_REQUIRED_RESULT MailCommon::FilterAction *action() const;

    void updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled);

Q_SIGNALS:
    void filterModified();
    void addFilterWidget(QWidget *);
    void removeFilterWidget(QWidget *);

private:
    //@cond PRIVATE
    class Private;
    Private *const d;
    //@endcond
};

/**
 * @short A container widget for a list of FilterActionWidgets.
 *
 * @author Marc Mutz <mutz@kde.org>
 * @see MailCommon::FilterAction MailCommon::MailFilter FilterActionWidget
 */
class MAILCOMMON_EXPORT FilterActionWidgetLister : public KPIM::KWidgetLister
{
    Q_OBJECT

public:
    /**
     * Creates a new filter action widget lister.
     *
     * @param parent The parent widget.
     */
    explicit FilterActionWidgetLister(QWidget *parent = nullptr);

    /**
     * Destroys the filter action widget lister.
     */
    ~FilterActionWidgetLister() override;

    /**
     * Sets the @p list of filter actions, the lister will create FilterActionWidgets for.
     */
    void setActionList(QVector<FilterAction *> *list);

    /**
     * Updates the action list according to the current action widget values.
     */
    void updateActionList();

    void reconnectWidget(FilterActionWidget *w);

public Q_SLOTS:
    /**
     * Resets the action widgets.
     */
    void reset();
    void slotAddWidget(QWidget *);
    void slotRemoveWidget(QWidget *);

Q_SIGNALS:
    void filterModified();

protected:
    /**
     * @copydoc KPIM::KWidgetLister::clearWidget
     */
    void clearWidget(QWidget *) override;

    /**
     * @copydoc KPIM::KWidgetLister::createWidget
     */
    QWidget *createWidget(QWidget *) override;

    void updateAddRemoveButton();

private:
    //@cond PRIVATE
    class Private;
    Private *const d;
    //@endcond
    void connectWidget(QWidget *widget, FilterAction *filterAction);
};
}

#endif
