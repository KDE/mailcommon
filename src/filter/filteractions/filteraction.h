/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTION_H
#define MAILCOMMON_FILTERACTION_H

#include "mailcommon_export.h"
#include "searchpattern.h"

#include "mailcommon/itemcontext.h"

#include <Collection>
#include <Item>

#include <KMime/KMimeMessage>
#include <KMime/MDN>

#include <QObject>

class QWidget;

namespace MailCommon
{
/**
 * @short Abstract base class for mail filter actions.
 *
 * Abstract base class for mail filter actions. All it can do is
 * hold a name (ie. type-string). There are several sub-classes that
 * inherit form this and are capable of providing parameter handling
 * (import/export as string, a widget to allow editing, etc.)
 *
 * @author Marc Mutz <mutz@kde.org>, based on work by Stefan Taferner <taferner@kde.org>.
 * @see Filter FilterMgr
 */
class MAILCOMMON_EXPORT FilterAction : public QObject
{
    Q_OBJECT
public:
    /**
     * Describes the possible return codes of filter processing:
     */
    enum ReturnCode {
        ErrorNeedComplete = 0x1, ///< Could not process because a complete message is needed.
        GoOn = 0x2, ///< Go on with applying filter actions.
        ErrorButGoOn = 0x4, ///< A non-critical error occurred
        ///  (e.g. an invalid address in the 'forward' action),
        ///   but processing should continue.
        CriticalError = 0x8 ///< A critical error occurred during processing
                            ///  (e.g. "disk full").
    };

    /**
     * Creates a new filter action.
     *
     * The action is initialized with an identifier @p name and
     * an i18n'd @p label.
     */
    FilterAction(const QString &name, const QString &label, QObject *parent = nullptr);

    /**
     * Destroys the filter action.
     */
    virtual ~FilterAction();

    /**
     * Returns i18n'd label, ie. the one which is presented in
     * the filter dialog.
     */
    Q_REQUIRED_RESULT QString label() const;

    /**
     * Returns identifier name, ie. the one under which it is
     * known in the config.
     */
    Q_REQUIRED_RESULT QString name() const;

    Q_REQUIRED_RESULT virtual QStringList sieveRequires() const;

    Q_REQUIRED_RESULT virtual QString sieveCode() const;

    /**
     * Execute action on given message (inside the item context).
     * Returns @p CriticalError if a
     * critical error has occurred (eg. disk full), @p ErrorButGoOn if
     * there was a non-critical error (e.g. invalid address in
     * 'forward' action), @p ErrorNeedComplete if a complete message
     * is required, @p GoOn if the message shall be processed by
     * further filters and @p Ok otherwise.
     */
    Q_REQUIRED_RESULT virtual ReturnCode process(ItemContext &context, bool applyOnOutbound) const = 0;

    /**
     * Returns the required part from the item that is needed for the action to
     * operate. See @ref MailCommon::SearchRule::RequiredPart */
    Q_REQUIRED_RESULT virtual SearchRule::RequiredPart requiredPart() const = 0;
    /**
     * Determines whether this action is valid. But this is just a
     * quick test. Eg., actions that have a mail address as parameter
     * shouldn't try real address validation, but only check if the
     * string representation is empty.
     */
    Q_REQUIRED_RESULT virtual bool isEmpty() const;

    Q_REQUIRED_RESULT virtual QString informationAboutNotValidAction() const;

    /**
     * Creates a widget for setting the filter action parameter. Also
     * sets the value of the widget.
     */
    Q_REQUIRED_RESULT virtual QWidget *createParamWidget(QWidget *parent) const;

    /**
     * The filter action shall set it's parameter from the widget's
     * contents. It is allowed that the value is read by the action
     * before this function is called.
     */
    virtual void applyParamWidgetValue(QWidget *paramWidget);

    /**
     * The filter action shall set it's widget's contents from it's
     * parameter.
     */
    virtual void setParamWidgetValue(QWidget *paramWidget) const;

    /**
     * The filter action shall clear it's parameter widget's
     * contents.
     */
    virtual void clearParamWidget(QWidget *paramWidget) const;

    /**
     * Read extra arguments from given string.
     */
    virtual void argsFromString(const QString &argsStr) = 0;

    /**
     * Read extra arguments from given string.
     * Return true if we need to update config file
     */
    Q_REQUIRED_RESULT virtual bool argsFromStringInteractive(const QString &argsStr, const QString &filterName);

    Q_REQUIRED_RESULT virtual QString argsAsStringReal() const;

    /**
     * Return extra arguments as string. Must not contain newlines.
     */
    Q_REQUIRED_RESULT virtual QString argsAsString() const = 0;

    /**
     * Returns a translated string describing this filter for visualization
     * purposes, e.g. in the filter log.
     */
    Q_REQUIRED_RESULT virtual QString displayString() const = 0;

    /**
     * Called from the filter when a folder is removed. Tests if the
     * folder @p aFolder is used and changes to @p aNewFolder in this
     * case. Returns true if a change was made.
     */
    Q_REQUIRED_RESULT virtual bool folderRemoved(const Akonadi::Collection &aFolder, const Akonadi::Collection &aNewFolder);

    /**
     * Static function that creates a filter action of this type.
     */
    static FilterAction *newAction();

    /**
     * Automates the sending of MDNs from filter actions.
     */
    static void sendMDN(const Akonadi::Item &item,
                        KMime::MDN::DispositionType d,
                        const QVector<KMime::MDN::DispositionModifier> &m = QVector<KMime::MDN::DispositionModifier>());

Q_SIGNALS:
    /**
     * Called to notify that the current FilterAction has had some
     * value modification
     */
    void filterActionModified();

private:
    const QString mName;
    const QString mLabel;
};
}

#endif
