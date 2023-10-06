/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

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
    explicit FilterActionWithTest(const QString &name, const QString &label, QObject *parent = nullptr);

    /**
     * @copydoc FilterAction::~FilterAction
     */
    ~FilterActionWithTest() override;

    /**
     * @copydoc FilterAction::isEmpty
     */
    [[nodiscard]] bool isEmpty() const override;

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

    /**
     * @copydoc FilterAction::argsFromString
     */
    void argsFromString(const QString &argsStr) override;

    /**
     * @copydoc FilterAction::argsAsString
     */
    [[nodiscard]] QString argsAsString() const override;

    /**
     * @copydoc FilterAction::displayString
     */
    [[nodiscard]] QString displayString() const override;

protected:
    QString mParameter;
};
}
