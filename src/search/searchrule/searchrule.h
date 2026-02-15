/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include "mailcommon_export.h"
#include <Akonadi/SearchQuery>

#include <Akonadi/Item>

class KConfigGroup;
namespace MailCommon
{
/*!
 * \class MailCommon::SearchRule
 * \inmodule MailCommon
 * \inheaderfile MailCommon/SearchRule
 *
 * \brief This class represents one search pattern rule.
 * Incoming mail is sent through the list of mail filter
 * rules before it is placed in the associated mail folder (usually "inbox").
 * This class represents one mail filter rule. It is also used to represent
 * a search rule as used by the search dialog and folders.
 */
class MAILCOMMON_EXPORT SearchRule
{
public:
    /*!
     * Defines a pointer to a search rule.
     */
    using Ptr = std::shared_ptr<SearchRule>;

    /*!
     * Describes operators for comparison of field and contents.
     *
     * If you change the order or contents of the enum: do not forget
     * to change funcConfigNames[], sFilterFuncList and matches()
     * in SearchRule, too.
     * Also, it is assumed that these functions come in pairs of logical
     * opposites (ie. "=" <-> "!=", ">" <-> "<=", etc.).
     */
    enum Function {
        FuncNone = -1,
        FuncContains = 0,
        FuncContainsNot,
        FuncEquals,
        FuncNotEqual,
        FuncRegExp,
        FuncNotRegExp,
        FuncIsGreater,
        FuncIsLessOrEqual,
        FuncIsLess,
        FuncIsGreaterOrEqual,
        FuncIsInAddressbook,
        FuncIsNotInAddressbook,
        FuncIsInCategory,
        FuncIsNotInCategory,
        FuncHasAttachment,
        FuncHasNoAttachment,
        FuncStartWith,
        FuncNotStartWith,
        FuncEndWith,
        FuncNotEndWith,
        FuncHasInvitation,
        FuncHasNoInvitation,
    };

    /*!
     * @enum RequiredPart
     * \brief Possible required parts.
     */
    enum RequiredPart {
        Envelope = 0, ///< Envelope
        Header, ///< Header
        CompleteMessage ///< Whole message
    };

    /*!
     * Creates new new search rule.
     *
     * \a field The field to search in.
     * \a function The function to use for searching.
     * \a contents The contents to search for.
     */
    explicit SearchRule(const QByteArray &field = QByteArray(), Function function = FuncContains, const QString &contents = QString());

    /*!
     * Creates a new search rule from an \a other rule.
     */
    SearchRule(const SearchRule &other);

    /*!
     * Initializes this rule with an \a other rule.
     */
    const SearchRule &operator=(const SearchRule &other);

    /*!
     * Creates a new search rule of a certain type by instantiating the
     * appropriate subclass depending on the \a field.
     *
     * \a field The field to search in.
     * \a function The function to use for searching.
     * \a contents The contents to search for.
     */
    static SearchRule::Ptr createInstance(const QByteArray &field = QByteArray(), Function function = FuncContains, const QString &contents = QString());

    /*!
     * Creates a new search rule of a certain type by instantiating the
     * appropriate subclass depending on the \a field.
     *
     * \a field The field to search in.
     * \a function The name of the function to use for searching.
     * \a contents The contents to search for.
     */
    static SearchRule::Ptr createInstance(const QByteArray &field, const char *function, const QString &contents);

    /*!
     * Creates a new search rule by cloning an \a other rule.
     */
    static SearchRule::Ptr createInstance(const SearchRule &other);

    /*!
     * Creates a new search rule by deserializing its structure from a data \a stream.
     */
    static SearchRule::Ptr createInstance(QDataStream &stream);

    /*!
     * Creates a new search rule from a given config \a group.
     *
     * \a group The config group to read the structure from.
     * \a index The identifier that is used to distinguish
     *              rules within a single config group.
     *
     * \
ote This function does no validation of the data obtained
     *       from the config file. You should call isEmpty yourself
     *       if you need valid rules.
     */
    static SearchRule::Ptr createInstanceFromConfig(const KConfigGroup &group, int index);

    /*!
     * Destroys the search rule.
     */
    virtual ~SearchRule();

    /*!
     * Tries to match the rule against the KMime::Message in the
     * given \a item.
     *
     * Returns true if the rule matched, false otherwise.
     *
     * \note Must be implemented by subclasses.
     *
     * \param item The Akonadi item to match against
     * \return True if the rule matches, false otherwise
     */
    virtual bool matches(const Akonadi::Item &item) const = 0;

    /*!
     * Determines whether the rule is worth considering.
     * It isn't if either the field is not set or the contents is empty.
     * The calling code should make sure that it's rule list contains
     * only non-empty rules, as matches doesn't check this.
     *
     * \return True if the rule is empty, false otherwise
     */
    virtual bool isEmpty() const = 0;

    /*!
     * Returns the required part from the item that is needed for the search to
     * operate. See \sa RequiredPart
     *
     * \return The required part for this rule
     */
    virtual SearchRule::RequiredPart requiredPart() const = 0;

    /*!
     * Saves the object into a given config \a group.
     *
     * \param group The config group.
     * \param index The identifier that is used to distinguish
     *              rules within a single config group.
     *
     * \note This function will happily write itself even when it's
     *       not valid, assuming higher layers to Do The Right Thing(TM).
     */
    void writeConfig(KConfigGroup &group, int index) const;

    /*!
     * Generates a Sieve script representation of this rule.
     *
     * \param requireModules The list of Sieve modules required by the script
     * \param code The generated Sieve script code
     */
    void generateSieveScript(QStringList &requireModules, QString &code);

    /*!
     * Sets the filter \a function of the rule.
     *
     * \param function The filter function to set
     */
    void setFunction(Function function);

    /*!
     * Returns the filter function of the rule.
     *
     * \return The current filter function
     */
    Function function() const;

    /*!
     * Sets the message header field \a name.
     *
     * \note Make sure the name contains no trailing ':'.
     *
     * \param name The header field name
     */
    void setField(const QByteArray &name);

    /*!
     * Returns the message header field name (without the trailing ':').
     *
     * There are also six pseudo-headers:
     * @li \<message\>: Try to match against the whole message.
     * @li \<body\>: Try to match against the body of the message.
     * @li \<any header\>: Try to match against any header field.
     * @li \<recipients\>: Try to match against both To: and Cc: header fields.
     * @li \<size\>: Try to match against size of message (numerical).
     * @li \<age in days\>: Try to match against age of message (numerical).
     * @li \<status\>: Try to match against status of message (status).
     * @li \<tag\>: Try to match against message tags.
     *
     * \return The header field name
     */
    [[nodiscard]] QByteArray field() const;

    /*!
     * Set the \a contents of the rule.
     *
     * This can be either a substring to search for in
     * or a regexp pattern to match against the header.
     *
     * \param contents The contents to search for
     */
    void setContents(const QString &contents);

    /*!
     * Returns the contents of the rule.
     *
     * \return The search contents
     */
    [[nodiscard]] QString contents() const;

    /*!
     * Returns the rule as string for debugging purpose
     *
     * \return A string representation of the rule
     */
    [[nodiscard]] const QString asString() const;

    /*!
     * Adds query terms to the given term group.
     *
     * \param groupTerm The search term group to add query terms to
     * \param emptyIsNotAnError Whether empty values should be considered an error
     */
    virtual void addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const
    {
        Q_UNUSED(groupTerm)
        Q_UNUSED(emptyIsNotAnError)
    }

    /*!
     * Stream operator for serialization.
     */
    QDataStream &operator>>(QDataStream &) const;
    /*!
     * Returns information about validation errors in this rule.
     *
     * \return A string describing validation errors, or empty if valid
     */
    virtual QString informationAboutNotValidRules() const
    {
        return {};
    }

protected:
    /*!
     * Helper that returns whether the rule has a negated function.
     *
     * \return True if the function is negated, false otherwise
     */
    [[nodiscard]] bool isNegated() const;

    /*!
     * Converts the rule function into the corresponding Akonadi query operator.
     *
     * \return The Akonadi search term condition
     */
    [[nodiscard]] Akonadi::SearchTerm::Condition akonadiComparator() const;

    /*!
     * Logs the result of a match operation for debugging purposes.
     *
     * \param result The result of the match operation
     */
    void maybeLogMatchResult(bool result) const;

private:
    MAILCOMMON_NO_EXPORT static Function configValueToFunc(const char *);
    MAILCOMMON_NO_EXPORT static QString functionToString(Function);
    MAILCOMMON_NO_EXPORT QString conditionToString(Function function);

    QByteArray mField;
    Function mFunction;
    QString mContents;
};
}
