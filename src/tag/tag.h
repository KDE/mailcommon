/* SPDX-FileCopyrightText: 2010 Thomas McGuire <mcguire@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/
#ifndef MAILCOMMONTAG_H
#define MAILCOMMONTAG_H

#include "mailcommon_export.h"

#include <QKeySequence>

#include <AkonadiCore/tag.h>
#include <QColor>
#include <QSharedPointer>

namespace MailCommon
{
// Our own copy of the tag data.
// Useful in the config dialog, because the user might cancel his changes,
// in which case we don't write them back.
// Also used as a convenience class in the TagActionManager.
class MAILCOMMON_EXPORT Tag
{
    Q_GADGET
public:
    using Ptr = QSharedPointer<Tag>;
    enum SaveFlag { TextColor = 1, BackgroundColor = 1 << 1, Font = 1 << 2 };
    using SaveFlags = QFlags<SaveFlag>;

    // Returns true if two tags are equal
    Q_REQUIRED_RESULT bool operator==(const Tag &other) const;

    Q_REQUIRED_RESULT bool operator!=(const Tag &other) const;

    static Ptr createDefaultTag(const QString &name);
    // expects a tag with all attributes fetched
    static Ptr fromAkonadi(const Akonadi::Tag &tag);

    Q_REQUIRED_RESULT Akonadi::Tag saveToAkonadi(SaveFlags saveFlags = SaveFlags(TextColor | BackgroundColor | Font)) const;

    // Compare, based on priority
    static bool compare(const Ptr &tag1, const Ptr &tag2);
    // Compare, based on name
    static bool compareName(const Ptr &tag1, const Ptr &tag2);

    qint64 id() const;
    QString name() const;
    Akonadi::Tag tag() const;

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

#endif
