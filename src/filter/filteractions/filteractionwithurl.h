/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteraction.h"
#include "mailcommon_private_export.h"
#include <QToolButton>
namespace MailCommon
{
/**
 * @short Abstract base class for filter actions with a command line as parameter.
 *
 * Abstract base class for mail filter actions that need a command
 * line as parameter, e.g. 'forward to'. Can create a QLineEdit
 * (are there better widgets in the depths of the kdelibs?) as
 * parameter widget. A subclass of this must provide at least
 * implementations for the following methods:
 *
 * @li virtual FilterAction::ReturnCodes FilterAction::process
 * @li static FilterAction::newAction
 *
 * The implementation of FilterAction::process should take the
 * command line specified in mParameter, make all required
 * modifications and stream the resulting command line into @p
 * mProcess. Then you can start the command with @p mProcess.start().
 *
 * @author Marc Mutz <mutz@kde.org>, based upon work by Stefan Taferner <taferner@kde.org>
 * @see FilterActionWithString FilterAction Filter KProcess
 */
class FilterActionWithUrlHelpButton : public QToolButton
{
    Q_OBJECT
public:
    explicit FilterActionWithUrlHelpButton(QWidget *parent = nullptr);
    ~FilterActionWithUrlHelpButton() override;
};

class MAILCOMMON_TESTS_EXPORT FilterActionWithUrl : public FilterAction
{
    Q_OBJECT
public:
    /**
     * @copydoc FilterAction::FilterAction
     */
    FilterActionWithUrl(const QString &name, const QString &label, QObject *parent = nullptr);

    /**
     * @copydoc FilterAction::~FilterAction
     */
    ~FilterActionWithUrl() override;

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
     * @copydoc FilterAction::applyFromString
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

private:
    mutable FilterActionWithUrlHelpButton *mHelpButton = nullptr;

private:
    void slotHelp();
};
}
