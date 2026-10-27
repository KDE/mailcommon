/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "mailcommon_export.h"
#include "searchpattern.h"

#include "mailcommon/itemcontext.h"

#include <Akonadi/Collection>
#include <Akonadi/Item>

#include <KMime/MDN>
#include <KMime/Message>

#include <QObject>

class QWidget;

namespace MailCommon
{
/*!
 * \class MailCommon::FilterAction
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FilterAction
 *
 * \brief Abstract base class for mail filter actions.
 *
 * Abstract base class for mail filter actions. All it can do is
 * hold a name (ie. type-string). There are several sub-classes that
 * inherit form this and are capable of providing parameter handling
 * (import/export as string, a widget to allow editing, etc.)
 *
 * \author Marc Mutz <mutz@kde.org>, based on work by Stefan Taferner <taferner@kde.org>.
 * \sa MailFilter FilterManager
 */
class MAILCOMMON_EXPORT FilterAction : public QObject
{
    Q_OBJECT
public:
    /*!
     * Describes the possible return codes of filter processing:
     *
     * \value ErrorNeedComplete Could not process because a complete message is needed.
     * \value GoOn Go on with applying filter actions.
     * \value ErrorButGoOn A non-critical error occurred (e.g. an invalid address in
     *        the 'forward' action), but processing should continue.
     * \value CriticalError A critical error occurred during processing (e.g. "disk full").
     */
    enum ReturnCode {
        ErrorNeedComplete = 0x1,
        GoOn = 0x2,
        ErrorButGoOn = 0x4,
        CriticalError = 0x8
    };

    /*!
     * Creates a new filter action.
     *
     * The action is initialized with an identifier \a name and
     * an i18n'd \a label, with the given \a parent.
     */
    FilterAction(const QString &name, const QString &label, QObject *parent = nullptr);

    /*!
     * Destroys the filter action.
     */
    ~FilterAction() override;

    /*!
     * Returns i18n'd label, ie. the one which is presented in
     * the filter dialog.
     */
    [[nodiscard]] QString label() const;

    /*!
     * Returns identifier name, ie. the one under which it is
     * known in the config.
     */
    [[nodiscard]] QString name() const;

    [[nodiscard]] virtual QStringList sieveRequires() const;

    [[nodiscard]] virtual QString sieveCode() const;

    /*!
     * Execute action on given message (inside the item \a context).
     * \a applyOnOutbound is \c true when the filter is applied on outgoing messages.
     * Returns \c CriticalError if a
     * critical error has occurred (eg. disk full), \c ErrorButGoOn if
     * there was a non-critical error (e.g. invalid address in
     * 'forward' action), \c ErrorNeedComplete if a complete message
     * is required, \c GoOn if the message shall be processed by
     * further filters and \c Ok otherwise.
     */
    [[nodiscard]] virtual ReturnCode process(ItemContext &context, bool applyOnOutbound) const = 0;

    /*!
     * Returns the required part from the item that is needed for the action to
     * operate. See \ MailCommon::SearchRule::RequiredPart */
    [[nodiscard]] virtual SearchRule::RequiredPart requiredPart() const = 0;
    /*!
     * Returns \c true if this action is empty, i.e. not valid. But this is just a
     * quick test. Eg., actions that have a mail address as parameter
     * shouldn't try real address validation, but only check if the
     * string representation is empty.
     */
    [[nodiscard]] virtual bool isEmpty() const;

    [[nodiscard]] virtual QString informationAboutNotValidAction() const;

    /*!
     * Creates a widget with the given \a parent for setting the filter action
     * parameter. Also sets the value of the widget.
     */
    [[nodiscard]] virtual QWidget *createParamWidget(QWidget *parent) const;

    /*!
     * The filter action shall set it's parameter from the contents of
     * \a paramWidget. It is allowed that the value is read by the action
     * before this function is called.
     */
    virtual void applyParamWidgetValue(QWidget *paramWidget);

    /*!
     * The filter action shall set the contents of \a paramWidget from it's
     * parameter.
     */
    virtual void setParamWidgetValue(QWidget *paramWidget) const;

    /*!
     * The filter action shall clear the contents of its parameter widget
     * \a paramWidget.
     */
    virtual void clearParamWidget(QWidget *paramWidget) const;

    /*!
     * Read extra arguments from the given string \a argsStr.
     */
    virtual void argsFromString(const QString &argsStr) = 0;

    /*!
     * Read extra arguments from the given string \a argsStr, for the filter
     * named \a filterName.
     * Returns \c true if we need to update config file.
     */
    [[nodiscard]] virtual bool argsFromStringInteractive(const QString &argsStr, const QString &filterName);

    [[nodiscard]] virtual QString argsAsStringReal() const;

    /*!
     * Return extra arguments as string. Must not contain newlines.
     */
    [[nodiscard]] virtual QString argsAsString() const = 0;

    /*!
     * Returns a translated string describing this filter for visualization
     * purposes, e.g. in the filter log.
     */
    [[nodiscard]] virtual QString displayString() const = 0;

    /*!
     * Called from the filter when a folder is removed. Tests if the
     * folder \a aFolder is used and changes to \a aNewFolder in this
     * case. Returns true if a change was made.
     */
    [[nodiscard]] virtual bool folderRemoved(const Akonadi::Collection &aFolder, const Akonadi::Collection &aNewFolder);

    /*!
     * Static function that creates a filter action of this type.
     */
    static FilterAction *newAction();

    /*!
     * Automates the sending of MDNs from filter actions: sends an MDN for
     * \a item with the disposition type \a d and the disposition modifiers \a m.
     */
    static void sendMDN(const Akonadi::Item &item,
                        KMime::MDN::DispositionType d,
                        const QList<KMime::MDN::DispositionModifier> &m = QList<KMime::MDN::DispositionModifier>());

Q_SIGNALS:
    /*!
     * Called to notify that the current FilterAction has had some
     * value modification
     */
    void filterActionModified();

private:
    const QString mName;
    const QString mLabel;
};
}
