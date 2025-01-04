/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithstring.h"
#include "mailcommon_private_export.h"
namespace MailCommon
{
/**
 * @short Abstract base class for filter actions with a mail address as parameter.
 *
 * Abstract base class for mail filter actions that need a mail
 * address as parameter, e.g. 'forward to'. Can create a
 * KComboBox (capable of completion from the address book) as
 * parameter widget. A subclass of this must provide at least
 * implementations for the following methods:
 *
 * @li virtual FilterAction::ReturnCodes FilterAction::process
 * @li static FilterAction::newAction
 *
 * @author Marc Mutz <mutz@kde.org>, based upon work by Stefan Taferner <taferner@kde.org>
 * @see FilterActionWithString FilterAction Filter
 */
class MAILCOMMON_TESTS_EXPORT FilterActionWithAddress : public FilterActionWithString
{
    Q_OBJECT

public:
    /**
     * @copydoc FilterAction::FilterAction
     */
    FilterActionWithAddress(const QString &name, const QString &label, QObject *parent = nullptr);

    /**
     * @copydoc FilterAction::createParamWidget
     */
    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;

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
};
}
