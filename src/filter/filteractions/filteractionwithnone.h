/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONWITHNONE_H
#define MAILCOMMON_FILTERACTIONWITHNONE_H

#include "filteraction.h"

namespace MailCommon
{
/**
 *  @short Abstract base class for filter actions with no parameter.
 *
 *  Abstract base class for mail filter actions that need no
 *  parameter, e.g. "Confirm Delivery". Creates an (empty) QWidget as
 *  parameter widget. A subclass of this must provide at least
 *  implementations for the following methods:
 *
 *  @li virtual FilterAction::ReturnCodes FilterAction::process
 *  @li static FilterAction::newAction
 *
 *  @author Marc Mutz <mutz@kde.org>, based upon work by Stefan Taferner <taferner@kde.org>
 *  @see FilterAction Filter
 */
class FilterActionWithNone : public FilterAction
{
    Q_OBJECT

public:
    /**
     * @copydoc FilterAction::FilterAction
     */
    FilterActionWithNone(const QString &name, const QString &label, QObject *parent = nullptr);

    /**
     * @copydoc FilterAction::argsFromString
     */
    void argsFromString(const QString &) override;

    /**
     * @copydoc FilterAction::argsAsString
     */
    Q_REQUIRED_RESULT QString argsAsString() const override;

    /**
     * @copydoc FilterAction::displayString
     */
    Q_REQUIRED_RESULT QString displayString() const override;
};
}

#endif
