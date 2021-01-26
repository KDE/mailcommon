/*
 * kmail: KDE mail client
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */
#ifndef MAILCOMMON_MAILFILTER_H
#define MAILCOMMON_MAILFILTER_H

#include "mailcommon_export.h"

#include "mailcommon/filteraction.h"
#include "mailcommon/searchpattern.h"

#include <AkonadiCore/collection.h>
#include <QKeySequence>

#include <kmime/kmime_message.h>

#include <QDataStream>
#include <QVector>

class KConfigGroup;

namespace MailCommon
{
/**
 * @brief The MailFilter class
 */
class MAILCOMMON_EXPORT MailFilter
{
    friend MAILCOMMON_EXPORT QDataStream &operator<<(QDataStream &stream, const MailFilter &filter);
    friend MAILCOMMON_EXPORT QDataStream &operator>>(QDataStream &stream, MailFilter &filter);

public:
    /** Result codes returned by process. They mean:

      @param GoOn Everything OK. You are still the owner of the
      message and you should continue applying filter actions to this
      message.

      @param CriticalError A critical error occurred (e.g. "disk full").

      @param NoResult For internal use only!

    */
    enum ReturnCode { NoResult, GoOn, CriticalError };

    /** Account type codes used by setApplicability. They mean:

      @param All Apply to all accounts

      @param ButImap Apply to all but IMAP accounts

      @param Checked apply to all accounts specified by setApplyOnAccount

    */
    enum AccountType { All, ButImap, Checked };

    /** Constructor that initializes basic settings. */
    MailFilter();

    /** Constructor that initializes from given config group.
     * Filters are stored one by one in config groups, i.e.
     * one filter, one group. */
    explicit MailFilter(const KConfigGroup &aConfig, bool internal, bool &needUpdate);

    /** Copy constructor. Constructs a deep copy of @p aFilter. */
    MailFilter(const MailFilter &other);

    /** Cleanup. */
    ~MailFilter();

    static int filterActionsMaximumSize();
    void generateRandomIdentifier();

    /**
     * Returns the unique identifier of this filter.
     */
    Q_REQUIRED_RESULT QString identifier() const;

    /** Equivalent to @p pattern()->name(). @return name of the filter */
    Q_REQUIRED_RESULT QString name() const;

    /** Execute the filter action(s) on the given message.
      Returns:
      @li 2 if a critical error occurred,
      @li 1 if the caller is still
      the owner of the message,
      @li 0 if processed successfully.
      @param context The context that contains the item to which the actions should be applied.
      @param stopIt Contains @c true if the caller may apply other filters and @c false if he shall
      stop the filtering of this message.
      @param applyOnOutbound Defines whether to apply the rules on the outbound.
    */
    Q_REQUIRED_RESULT ReturnCode execActions(ItemContext &context, bool &stopIt, bool applyOnOutbound) const;

    /**
     * Returns the required part from the item that is needed for the filter to
     * operate. See @ref SearchRule::RequiredPart */
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart(const QString &id) const;

    /** Write contents to given config group. */
    void writeConfig(KConfigGroup &config, bool exportFilter) const;

    /** Initialize from given config group. */
    Q_REQUIRED_RESULT bool readConfig(const KConfigGroup &config, bool interactive = false);

    /** Remove empty rules (and actions one day). */
    QString purify(bool removeAction = true);

    /** Check for empty pattern and action list. */
    bool isEmpty() const;

    /** Provides a reference to the internal action list. If your used
      the @p setAction() and @p action() functions before, please
      convert to using myFilter->actions()->at() and friends now. */
    QVector<FilterAction *> *actions();

    /** Provides a reference to the internal action list. Const version. */
    const QVector<FilterAction *> *actions() const;

    /** Provides a reference to the internal pattern. If you used the
      @p matches() function before, please convert to using
      myFilter->pattern()->matches() now. */
    Q_REQUIRED_RESULT SearchPattern *pattern();

    /** Provides a reference to the internal pattern. If you used the
      @p matches() function before, please convert to using
      myFilter->pattern()->matches() now. */
    const SearchPattern *pattern() const;

    /** Set whether this filter should be applied on
      outbound messages (@p aApply == true) or not.
      See applyOnOutbound applyOnInbound setApplyOnInbound
    */
    void setApplyOnOutbound(bool aApply);

    /** Set whether this filter should be applied on
      outbound messages before sending (@p aApply == TRUE) or not.
      See applyOnOutbound applyOnInbound setApplyOnInbound
    */
    void setApplyBeforeOutbound(bool aApply);

    /** @return true if this filter should be applied on
      outbound messages, false otherwise.
      @see setApplyOnOutbound applyOnInbound setApplyOnInbound
    */
    bool applyOnOutbound() const;

    /** @return TRUE if this filter should be applied on
      outbound messages before they are sent, FALSE otherwise.
      @see setApplyOnOutbound applyOnInbound setApplyOnInbound
    */
    bool applyBeforeOutbound() const;

    /** Set whether this filter should be applied on
      inbound messages (@p aApply == true) or not.
      @see setApplyOnOutbound applyOnInbound applyOnOutbound
    */
    void setApplyOnInbound(bool aApply);

    /** @return true if this filter should be applied on
      inbound messages, false otherwise.
      @see setApplyOnOutbound applyOnOutbound setApplyOnInbound
    */
    bool applyOnInbound() const;

    /** Set whether this filter should be applied on
      explicit (CTRL-J) filtering (@p aApply == true) or not.
      @see setApplyOnOutbound applyOnInbound applyOnOutbound
    */
    void setApplyOnExplicit(bool aApply);

    /** @return true if this filter should be applied on
      explicit (CTRL-J) filtering, false otherwise.
      @see setApplyOnOutbound applyOnOutbound setApplyOnInbound
    */
    bool applyOnExplicit() const;

    /** Set whether this filter should be applied on
      inbound messages for all accounts (@p aApply == All) or
      inbound messages for all but IMAP accounts (@p aApply == ButImap) or
      for a specified set of accounts only.
      Only applicable to filters that are applied on inbound messages.
      @see setApplyOnInbound setApplyOnAccount
    */
    void setApplicability(AccountType aApply = All);

    /** Sets whether the filter should be applied on inbound emails in all
      folders, not just Inbox.
    */
    void setApplyOnAllFoldersInbound(bool aApply);

    /** Returns whether the filter should be applied on inbound emails in all
      folders, not just Inbox.
    */
    bool applyOnAllFoldersInbound() const;

    /** @return true if this filter should be applied on
      inbound messages for all accounts, or false if this filter
      is to be applied on a specified set of accounts only.
      Only applicable to filters that are applied on inbound messages.
      @see setApplicability
    */
    AccountType applicability() const;

    /** Set whether this filter should be applied on
      inbound messages for the account with id (@p id).
      Only applicable to filters that are only applied to a specified
      set of accounts.
      @see setApplicability applyOnAccount
    */
    void setApplyOnAccount(const QString &id, bool aApply = true);

    /** @return true if this filter should be applied on
      inbound messages from the account with id (@p id), false otherwise.
      @see setApplicability
    */
    bool applyOnAccount(const QString &id) const;

    void setStopProcessingHere(bool aStop);
    bool stopProcessingHere() const;

    /** Set whether this filter should be plugged into the filter menu.
     */
    void setConfigureShortcut(bool aShort);

    /** @return true if this filter should be plugged into the filter menu,
      false otherwise.
      @see setConfigureShortcut
    */
    bool configureShortcut() const;

    /** Set whether this filter should be plugged into the toolbar.
      This can be done only if a shortcut is defined.
      @see setConfigureShortcut
    */
    void setConfigureToolbar(bool aTool);

    /** @return true if this filter should be plugged into the toolbar,
      false otherwise.
      @see setConfigureToolbar
    */
    bool configureToolbar() const;

    /** @return The toolbar name of this filter.
     *  @see setToolbarName
     */
    QString toolbarName() const;

    /** This sets the toolbar name for this filter.
     *  The toolbar name is the text to be displayed underneath the toolbar icon
     *  for this filter. This is usually the same as name(),  expect when
     *  explicitly set by this function.
     *  This is useful if the normal filter mame is too long for the toolbar.
     *  @see toolbarName, name
     */
    void setToolbarName(const QString &toolbarName);

    /** Set the shortcut to be used if plugged into the filter menu
      or toolbar. Default is no shortcut.
      @see setConfigureShortcut setConfigureToolbar
    */
    void setShortcut(const QKeySequence &shortcut);

    /** @return The shortcut assigned to the filter.
      @see setShortcut
    */
    const QKeySequence &shortcut() const;

    /** Set the icon to be used if plugged into the filter menu
      or toolbar. Default is the gear icon.
      @see setConfigureShortcut setConfigureToolbar
    */
    void setIcon(const QString &icon);

    /** @return The name of the icon to be used.
      @see setIcon
    */
    QString icon() const;

    /**
     * Called from the filter manager when a folder is moved.
     * Tests if the folder aFolder is used in any action. Changes it
     * to aNewFolder folder in this case.
     * @return true if a change in some action occurred,
     * false if no action was affected.
     */
    void folderRemoved(const Akonadi::Collection &aFolder, const Akonadi::Collection &aNewFolder);

    /** Returns the filter in a human-readable form. useful for
      debugging but not much else. Don't use, as it may well go away
      in the future... */
    const QString asString() const;

    /** Set the mode for using automatic naming for the filter.
      If the feature is enabled, the name is derived from the
      first filter rule.
    */
    void setAutoNaming(bool useAutomaticNames);

    /** @return Tells, if an automatic name is used for the filter
     */
    bool isAutoNaming() const;

    /** Return if filter is enabled or not
     */
    bool isEnabled() const;
    void setEnabled(bool);

    void generateSieveScript(QStringList &requiresModules, QString &code);

    void clearApplyOnAccount();
    void agentRemoved(const QString &identifier);

private:
    QString mIdentifier;
    SearchPattern mPattern;
    QVector<FilterAction *> mActions;
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

#endif /*MAILCOMMON_MAILFILTER_H*/
