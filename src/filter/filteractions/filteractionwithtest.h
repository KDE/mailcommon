/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONWITHTEST_H
#define MAILCOMMON_FILTERACTIONWITHTEST_H

#include "filteraction.h"

namespace MailCommon
{
class FilterActionWithTest : public FilterAction
{
    Q_OBJECT
public:
    /**
     * @copydoc FilterAction::FilterAction
     */
    FilterActionWithTest(const QString &name, const QString &label, QObject *parent = nullptr);

    /**
     * @copydoc FilterAction::~FilterAction
     */
    ~FilterActionWithTest() override;

    /**
     * @copydoc FilterAction::isEmpty
     */
    Q_REQUIRED_RESULT bool isEmpty() const override;

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

    /**
     * @copydoc FilterAction::argsAsString
     */
    Q_REQUIRED_RESULT QString argsAsString() const override;

    /**
     * @copydoc FilterAction::displayString
     */
    Q_REQUIRED_RESULT QString displayString() const override;

protected:
    QString mParameter;
};
}

#endif
