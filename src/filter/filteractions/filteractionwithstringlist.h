/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONWITHSTRINGLIST_H
#define MAILCOMMON_FILTERACTIONWITHSTRINGLIST_H

#include "filteractionwithstring.h"

#include <QStringList>

namespace MailCommon
{
/**
 * @short Abstract base class for filter actions with a fixed set of string parameters.
 *
 * Abstract base class for mail filter actions that need a
 * parameter which can be chosen from a fixed set, e.g. 'set
 * identity'.  Can create a KComboBox as parameter widget. A
 * subclass of this must provide at least implementations for the
 * following methods:
 *
 * @li virtual FilterAction::ReturnCodes FilterAction::process
 * @li static  FilterAction::newAction
 *
 * Additionally, it's constructor should populate the
 * QStringList @p mParameterList with the valid parameter
 * strings. The combobox will then contain be populated automatically
 * with those strings. The default string will be the first one.
 *
 * @author Marc Mutz <mutz@kde.org>, based upon work by Stefan Taferner <taferner@kde.org>
 * @see FilterActionWithString FilterActionWithFolder FilterAction Filter
 */
class FilterActionWithStringList : public FilterActionWithString
{
    Q_OBJECT

public:
    /**
     * @copydoc FilterAction::FilterAction
     */
    FilterActionWithStringList(const QString &name, const QString &label, QObject *parent = nullptr);

    /**
     * @copydoc FilterAction::createParamWidget
     */
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;

    /**
     * @copydoc FilterAction::applyParamWidgetValue
     */
    void applyParamWidgetValue(QWidget *paramWidget) override;

    /**
     * @copydoc FilterAction::setParamWidgetValue
     */
    void setParamWidgetValue(QWidget *paramWidget) const override;

    /**
     * @copydoc FilterAction::clearParamWidget
     */
    void clearParamWidget(QWidget *paramWidget) const override;

    /**
     * @copydoc FilterAction::argsFromString
     */
    void argsFromString(const QString &argsStr) override;

protected:
    QStringList mParameterList;
};
}

#endif
