/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONSETTRANSPORT_H
#define MAILCOMMON_FILTERACTIONSETTRANSPORT_H

#include "filteraction.h"

namespace MailCommon
{
//=============================================================================
// FilterActionSetTransport - set transport to...
// Specify mail transport (smtp server) to be used when replying to a message
//=============================================================================
class FilterActionSetTransport : public FilterAction
{
    Q_OBJECT
public:
    explicit FilterActionSetTransport(QObject *parent = nullptr);
    ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    QWidget *createParamWidget(QWidget *parent) const override;
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

    bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;

    /**
     * @copydoc FilterAction::argsFromString
     */
    void argsFromString(const QString &argsStr) override;
    /**
     * @copydoc FilterAction::isEmpty
     */
    Q_REQUIRED_RESULT bool isEmpty() const override;
    /**
     * @copydoc FilterAction::argsAsString
     */
    Q_REQUIRED_RESULT QString argsAsString() const override;

    /**
     * @copydoc FilterAction::displayString
     */
    Q_REQUIRED_RESULT QString displayString() const override;
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;

protected:
    int mParameter;
    mutable QString mTransportName;
};
}

#endif
