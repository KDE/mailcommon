/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONWITHUOID_H
#define MAILCOMMON_FILTERACTIONWITHUOID_H

#include "filteraction.h"

namespace MailCommon
{
/**
 * @short Abstract base class for filter actions with a free-form string as parameter.
 *
 * Abstract base class for mail filter actions that need a
 * parameter that has a UOID, e.g. "set identity". A subclass of this
 * must provide at least implementations for the following methods:
 *
 * @li virtual FilterAction::ReturnCodes FilterAction::process
 * @li static FilterAction::newAction
 * @li the *ParamWidget* methods.
 *
 * @author Marc Mutz <mutz@kde.org>, based upon work by Stefan Taferner <taferner@kde.org>
 * @see FilterAction Filter
 */
class FilterActionWithUOID : public FilterAction
{
    Q_OBJECT

public:
    /**
     * @copydoc FilterAction::FilterAction
     */
    FilterActionWithUOID(const QString &name, const QString &label, QObject *parent = nullptr);

    /**
     * @copydoc FilterAction::isEmpty
     */
    Q_REQUIRED_RESULT bool isEmpty() const override;

    /**
     * @copydoc FilterAction::argsFromString
     */
    void argsFromString(const QString &argsStr) override;

    /**
     * @copydoc FilterAction::argsAsString
     */
    Q_REQUIRED_RESULT QString argsAsString() const override;

    /**
     * @copydoc FilterAction::displayString
     */
    Q_REQUIRED_RESULT QString displayString() const override;

protected:
    uint mParameter;
};
}

#endif
