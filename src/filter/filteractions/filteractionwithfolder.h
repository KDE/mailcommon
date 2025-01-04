/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteraction.h"
#include "mailcommon_private_export.h"
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
class MAILCOMMON_TESTS_EXPORT FilterActionWithFolder : public FilterAction
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

    [[nodiscard]] bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;

    [[nodiscard]] QString argsAsStringReal() const override;

    /**
     * @copydoc FilterAction::displayString
     */
    [[nodiscard]] QString displayString() const override;

    /**
     * @copydoc FilterAction::folderRemoved
     */
    [[nodiscard]] bool folderRemoved(const Akonadi::Collection &aFolder, const Akonadi::Collection &aNewFolder) override;

protected:
    Akonadi::Collection mFolder;
};
}
