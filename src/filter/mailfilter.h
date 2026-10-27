/*
 * kmail: KDE mail client
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */
#pragma once

#include "mailcommon_export.h"

#include "mailcommon/filteraction.h"
#include "mailcommon/searchpattern.h"

#include <Akonadi/Collection>
#include <QKeySequence>

#include <QDataStream>
#include <QList>

class KConfigGroup;

namespace MailCommon
{
/*!
 * \class MailCommon::MailFilter
 * \inmodule MailCommon
 * \inheaderfile MailCommon/MailFilter
 *
 * \brief The MailFilter class.
 */
class MAILCOMMON_EXPORT MailFilter
{
    friend MAILCOMMON_EXPORT QDataStream &operator<<(QDataStream &stream, const MailFilter &filter);
    friend MAILCOMMON_EXPORT QDataStream &operator>>(QDataStream &stream, MailFilter &filter);

public:
    /*! Result codes returned by process.

      \value NoResult For internal use only!
      \value GoOn Everything OK. You are still the owner of the
      message and you should continue applying filter actions to this
      message.
      \value CriticalError A critical error occurred (e.g. "disk full").
    */
    enum ReturnCode {
        NoResult,
        GoOn,
        CriticalError
    };

    /*! Account type codes used by setApplicability.

      \value All Apply to all accounts
      \value ButImap Apply to all but IMAP accounts
      \value Checked Apply to all accounts specified by setApplyOnAccount
    */
    enum AccountType {
        All,
        ButImap,
        Checked,
    };

    /*! Constructor that initializes basic settings. */
    MailFilter();

    /*! Constructor that initializes from the given config group \a aConfig.
     * Filters are stored one by one in config groups, i.e.
     * one filter, one group. If \a interactive is \c true, the user may be asked
     * to fix the filter. \a needUpdate is set to \c true if the configuration
     * needs to be written back. */
    explicit MailFilter(const KConfigGroup &aConfig, bool interactive, bool &needUpdate);

    /*! Copy constructor. Constructs a deep copy of \a other. */
    MailFilter(const MailFilter &other);

    /*! Cleanup. */
    ~MailFilter();

    /*!
     * Returns the maximum number of actions of a filter.
     */
    static int filterActionsMaximumSize();
    /*!
     * Generates a new random identifier for this filter.
     */
    void generateRandomIdentifier();

    /*!
     * Returns the unique identifier of this filter.
     */
    [[nodiscard]] QString identifier() const;

    /*! Equivalent to \c {pattern()->name()}. Returns name of the filter */
    [[nodiscard]] QString name() const;

    /*! Execute the filter action(s) on the given message.
      Returns:
      \list
      \li 2 if a critical error occurred,
      \li 1 if the caller is still
      the owner of the message,
      \li 0 if processed successfully.
      \endlist
      \a context The context that contains the item to which the actions should be applied.
      \a stopIt Contains \c true if the caller may apply other filters and \c false if he shall
      stop the filtering of this message.
      \a applyOnOutbound Defines whether to apply the rules on the outbound.
    */
    [[nodiscard]] ReturnCode execActions(ItemContext &context, bool &stopIt, bool applyOnOutbound) const;

    /*!
     * Returns the required part from the item that is needed for the filter to
     * operate on messages from the account \a id. See SearchRule::RequiredPart */
    [[nodiscard]] SearchRule::RequiredPart requiredPart(const QString &id) const;

    /*! Write contents to the given \a config group. If \a exportFilter is \c true,
     *  the action arguments are written in their exportable form. */
    void writeConfig(KConfigGroup &config, bool exportFilter) const;

    /*! Initialize from the given \a config group. If \a interactive is \c true,
     *  the user may be asked to fix the filter.
     *  Returns \c true if the configuration needs to be written back. */
    [[nodiscard]] bool readConfig(const KConfigGroup &config, bool interactive = false);

    /*! Remove empty rules, and empty actions if \a removeAction is \c true.
     *  Returns a description of the invalid rules and actions. */
    QString purify(bool removeAction = true);

    /*! Returns \c true if the pattern and the action list are empty. */
    bool isEmpty() const;

    /*! Provides a reference to the internal action list. If your used
      the \c setAction() and \c action() functions before, please
      convert to using myFilter->actions()->at() and friends now. */
    QList<FilterAction *> *actions();

    /*! Provides a reference to the internal action list. Const version. */
    const QList<FilterAction *> *actions() const;

    /*! Provides a reference to the internal pattern. If you used the
      \c matches() function before, please convert to using
      myFilter->pattern()->matches() now. */
    [[nodiscard]] SearchPattern *pattern();

    /*! Provides a reference to the internal pattern. If you used the
      \c matches() function before, please convert to using
      myFilter->pattern()->matches() now. */
    const SearchPattern *pattern() const;

    /*! Set whether this filter should be applied on
      outbound messages (\a apply == true) or not.
      See applyOnOutbound applyOnInbound setApplyOnInbound
    */
    void setApplyOnOutbound(bool apply);

    /*! Set whether this filter should be applied on
      outbound messages before sending (\a apply == TRUE) or not.
      See applyOnOutbound applyOnInbound setApplyOnInbound
    */
    void setApplyBeforeOutbound(bool apply);

    /*! Returns true if this filter should be applied on
      outbound messages, false otherwise.
      \sa setApplyOnOutbound applyOnInbound setApplyOnInbound
    */
    [[nodiscard]] bool applyOnOutbound() const;

    /*! Returns TRUE if this filter should be applied on
      outbound messages before they are sent, FALSE otherwise.
      \sa setApplyOnOutbound applyOnInbound setApplyOnInbound
    */
    [[nodiscard]] bool applyBeforeOutbound() const;

    /*! Set whether this filter should be applied on
      inbound messages (\a apply == true) or not.
      \sa setApplyOnOutbound applyOnInbound applyOnOutbound
    */
    void setApplyOnInbound(bool apply);

    /*! Returns true if this filter should be applied on
      inbound messages, false otherwise.
      \sa setApplyOnOutbound applyOnOutbound setApplyOnInbound
    */
    [[nodiscard]] bool applyOnInbound() const;

    /*! Set whether this filter should be applied on
      explicit (CTRL-J) filtering (\a apply == true) or not.
      \sa setApplyOnOutbound applyOnInbound applyOnOutbound
    */
    void setApplyOnExplicit(bool apply);

    /*! Returns true if this filter should be applied on
      explicit (CTRL-J) filtering, false otherwise.
      \sa setApplyOnOutbound applyOnOutbound setApplyOnInbound
    */
    [[nodiscard]] bool applyOnExplicit() const;

    /*! Set whether this filter should be applied on
      inbound messages for all accounts (\a apply == All) or
      inbound messages for all but IMAP accounts (\a apply == ButImap) or
      for a specified set of accounts only.
      Only applicable to filters that are applied on inbound messages.
      \sa setApplyOnInbound setApplyOnAccount
    */
    void setApplicability(AccountType apply = All);

    /*! Sets whether the filter should be applied on inbound emails in all
      folders, not just Inbox, according to \a apply.
    */
    void setApplyOnAllFoldersInbound(bool apply);

    /*! Returns whether the filter should be applied on inbound emails in all
      folders, not just Inbox.
    */
    [[nodiscard]] bool applyOnAllFoldersInbound() const;

    /*! Returns true if this filter should be applied on
      inbound messages for all accounts, or false if this filter
      is to be applied on a specified set of accounts only.
      Only applicable to filters that are applied on inbound messages.
      \sa setApplicability
    */
    [[nodiscard]] AccountType applicability() const;

    /*! Set whether this filter should be applied on
      inbound messages for the account with id (\a id), according to \a apply.
      Only applicable to filters that are only applied to a specified
      set of accounts.
      \sa setApplicability applyOnAccount
    */
    void setApplyOnAccount(const QString &id, bool apply = true);

    /*! Returns true if this filter should be applied on
      inbound messages from the account with id (\a id), false otherwise.
      \sa setApplicability
    */
    [[nodiscard]] bool applyOnAccount(const QString &id) const;

    void setStopProcessingHere(bool aStop);
    bool stopProcessingHere() const;

    /*! Set whether this filter should be plugged into the filter menu,
     *  according to \a aShort.
     */
    void setConfigureShortcut(bool aShort);

    /*! Returns true if this filter should be plugged into the filter menu,
      false otherwise.
      \sa setConfigureShortcut
    */
    [[nodiscard]] bool configureShortcut() const;

    /*! Set whether this filter should be plugged into the toolbar,
      according to \a aTool.
      This can be done only if a shortcut is defined.
      \sa setConfigureShortcut
    */
    void setConfigureToolbar(bool aTool);

    /*! Returns true if this filter should be plugged into the toolbar,
      false otherwise.
      \sa setConfigureToolbar
    */
    [[nodiscard]] bool configureToolbar() const;

    /*! Returns The toolbar name of this filter.
     *  \sa setToolbarName
     */
    QString toolbarName() const;

    /*! This sets the toolbar name for this filter to \a toolbarName.
     *  The toolbar name is the text to be displayed underneath the toolbar icon
     *  for this filter. This is usually the same as name(),  expect when
     *  explicitly set by this function.
     *  This is useful if the normal filter mame is too long for the toolbar.
     *  \sa toolbarName, name
     */
    void setToolbarName(const QString &toolbarName);

    /*! Set the \a shortcut to be used if plugged into the filter menu
      or toolbar. Default is no shortcut.
      \sa setConfigureShortcut setConfigureToolbar
    */
    void setShortcut(const QKeySequence &shortcut);

    /*! Returns The shortcut assigned to the filter.
      \sa setShortcut
    */
    const QKeySequence &shortcut() const;

    /*! Set the \a icon to be used if plugged into the filter menu
      or toolbar. Default is the gear icon.
      \sa setConfigureShortcut setConfigureToolbar
    */
    void setIcon(const QString &icon);

    /*! Returns The name of the icon to be used.
      \sa setIcon
    */
    [[nodiscard]] QString icon() const;

    /*!
     * Called from the filter manager when a folder is moved.
     * Tests if the folder \a aFolder is used in any action. Changes it
     * to \a aNewFolder folder in this case.
     */
    void folderRemoved(const Akonadi::Collection &aFolder, const Akonadi::Collection &aNewFolder);

    /*! Returns the filter in a human-readable form. useful for
      debugging but not much else. Don't use, as it may well go away
      in the future... */
    const QString asString() const;

    /*! Set the mode for using automatic naming for the filter to
      \a useAutomaticNames. If the feature is enabled, the name is derived from the
      first filter rule.
    */
    void setAutoNaming(bool useAutomaticNames);

    /*! Returns Tells, if an automatic name is used for the filter
     */
    [[nodiscard]] bool isAutoNaming() const;

    /*! Return if filter is enabled or not
     */
    [[nodiscard]] bool isEnabled() const;
    /*!
     * Enables or disables the filter.
     */
    void setEnabled(bool);

    /*!
     * Generates the Sieve script \a code of this filter, and appends the
     * required Sieve modules to \a requiresModules.
     */
    void generateSieveScript(QStringList &requiresModules, QString &code);

    /*!
     * Clears the list of accounts this filter applies to.
     */
    void clearApplyOnAccount();
    /*!
     * Called when the agent \a identifier is removed: the filter no longer
     * applies to this account.
     */
    void agentRemoved(const QString &identifier);

private:
    QString mIdentifier;
    SearchPattern mPattern;
    QList<FilterAction *> mActions;
    QStringList mAccounts;
    QString mIcon;
    QString mToolbarName;
    QKeySequence mShortcut;
    bool bApplyOnInbound : 1;
    bool bApplyBeforeOutbound : 1;
    bool bApplyOnOutbound : 1;
    bool bApplyOnExplicit : 1;
    bool bApplyOnAllFolders : 1;
    bool bStopProcessingHere : 1;
    bool bConfigureShortcut : 1;
    bool bConfigureToolbar : 1;
    bool bAutoNaming : 1;
    bool bEnabled : 1;
    AccountType mApplicability;
};

MAILCOMMON_EXPORT QDataStream &operator<<(QDataStream &stream, const MailFilter &filter);
MAILCOMMON_EXPORT QDataStream &operator>>(QDataStream &stream, MailFilter &filter);
}
