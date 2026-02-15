/* SPDX-FileCopyrightText: 2010 Thomas McGuire <mcguire@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/
#pragma once

#include "mailcommon_export.h"

#include <QKeySequence>

#include <Akonadi/Tag>
#include <QColor>
#include <QSharedPointer>

namespace MailCommon
{
// Our own copy of the tag data.
// Useful in the config dialog, because the user might cancel his changes,
// in which case we don't write them back.
// Also used as a convenience class in the TagActionManager.
/*!
 * \class MailCommon::Tag
 * \inmodule MailCommon
 * \inheaderfile MailCommon/Tag
 *
 * \brief The Tag class
 */
class MAILCOMMON_EXPORT Tag
{
    Q_GADGET
public:
    using Ptr = QSharedPointer<Tag>;
    enum SaveFlag {
        TextColor = 1,
        BackgroundColor = 1 << 1,
        Font = 1 << 2
    };
    using SaveFlags = QFlags<SaveFlag>;

    // Returns true if two tags are equal
    /*!
     * Compares this tag with another tag for equality.
     *
     * \param other The other tag to compare with
     * \return True if the tags are equal, false otherwise
     */
    [[nodiscard]] bool operator==(const Tag &other) const;

    /*!
     * Compares this tag with another tag for inequality.
     *
     * \param other The other tag to compare with
     * \return True if the tags are not equal, false otherwise
     */
    [[nodiscard]] bool operator!=(const Tag &other) const;

    /*!
     * Creates a default tag with the specified name.
     *
     * \param name The name of the new tag
     * \return A shared pointer to the newly created tag
     */
    static Ptr createDefaultTag(const QString &name);
    // expects a tag with all attributes fetched
    /*!
     * Creates a Tag from an Akonadi::Tag object.
     *
     * \param tag The Akonadi tag to convert
     * \return A shared pointer to the newly created Tag
     */
    static Ptr fromAkonadi(const Akonadi::Tag &tag);

    /*!
     * Converts this tag to an Akonadi::Tag object.
     *
     * \param saveFlags Flags indicating which attributes to save
     * \return An Akonadi::Tag containing the tag's data
     */
    [[nodiscard]] Akonadi::Tag saveToAkonadi(SaveFlags saveFlags = SaveFlags(TextColor | BackgroundColor | Font)) const;

    // Compare, based on priority
    /*!
     * Compares two tags based on their priority.
     *
     * \param tag1 The first tag to compare
     * \param tag2 The second tag to compare
     * \return True if tag1 has higher priority than tag2
     */
    static bool compare(const Ptr &tag1, const Ptr &tag2);
    // Compare, based on name
    /*!
     * Compares two tags alphabetically by name.
     *
     * \param tag1 The first tag to compare
     * \param tag2 The second tag to compare
     * \return True if tag1's name comes before tag2's name alphabetically
     */
    static bool compareName(const Ptr &tag1, const Ptr &tag2);

    /*!
     * Returns the unique identifier for this tag.
     *
     * \return The tag's ID
     */
    [[nodiscard]] qint64 id() const;
    /*!
     * Returns the human-readable name of this tag.
     *
     * \return The tag's name
     */
    [[nodiscard]] QString name() const;
    /*!
     * Returns the underlying Akonadi::Tag object.
     *
     * \return The Akonadi tag
     */
    [[nodiscard]] Akonadi::Tag tag() const;

    QString tagName;
    QColor textColor;
    QColor backgroundColor;
    QString iconName;
    QKeySequence shortcut;
    bool isBold;
    bool isItalic;
    bool inToolbar;
    bool isImmutable;
    // Priority, i.e. sort order of the tag. Only used when loading the tag, when saving
    // the priority is set to the position in the list widget
    int priority;

private:
    Tag()
        : isBold(false)
        , isItalic(false)
        , inToolbar(false)
        , isImmutable(false)
        , priority(0)
    {
    }

    Akonadi::Tag mTag;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Tag::SaveFlags)
}
