/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONWITHFOLDER_H
#define MAILCOMMON_FILTERACTIONWITHFOLDER_H

#include "filteraction.h"

namespace MailCommon
{
/**
 * @short Abstract base class for filter actions with a mail folder as parameter.
 *
 * Abstract base class for mail filter actions that need a
 * mail folder as parameter, e.g. 'move into folder'. Can
 * create a KComboBox as parameter widget. A subclass of this
 * must provide at least implementations for the following methods:
 *
 * @li virtual FilterAction::ReturnCodes FilterAction::process
 * @li static FilterAction::newAction
 *
 * @author Marc Mutz <mutz@kde.org>, based upon work by Stefan Taferner <taferner@kde.org>
 * @see FilterActionWithStringList FilterAction Filter
 */
class FilterActionWithFolder : public FilterAction
{
    Q_OBJECT

public:
    /**
     * @copydoc FilterAction::FilterAction
     */
    FilterActionWithFolder(const QString &name, const QString &label, QObject *parent = nullptr);

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

    bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;

    Q_REQUIRED_RESULT QString argsAsStringReal() const override;

    /**
     * @copydoc FilterAction::displayString
     */
    Q_REQUIRED_RESULT QString displayString() const override;

    /**
     * @copydoc FilterAction::folderRemoved
     */
    Q_REQUIRED_RESULT bool folderRemoved(const Akonadi::Collection &aFolder, const Akonadi::Collection &aNewFolder) override;

protected:
    Akonadi::Collection mFolder;
};
}

#endif
