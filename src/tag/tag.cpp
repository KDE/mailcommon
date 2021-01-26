/* SPDX-FileCopyrightText: 2010 Thomas McGuire <mcguire@kde.org>
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/
#include "tag.h"

#include <QFont>
#include <QGuiApplication>
#include <Tag>
#include <TagAttribute>

#include <QUuid>
using namespace MailCommon;

Tag::Ptr Tag::createDefaultTag(const QString &name)
{
    Tag::Ptr tag(new Tag());
    tag->tagName = name;
    tag->iconName = QStringLiteral("mail-tagged");

    tag->priority = -1;
    tag->inToolbar = false;
    tag->isImmutable = false;
    tag->isBold = false;
    tag->isItalic = false;
    return tag;
}

Tag::Ptr Tag::fromAkonadi(const Akonadi::Tag &akonadiTag)
{
    Tag::Ptr tag(new Tag());
    tag->tagName = akonadiTag.name();
    tag->mTag = akonadiTag;
    tag->priority = -1;
    tag->iconName = QStringLiteral("mail-tagged");
    tag->inToolbar = false;
    tag->isImmutable = akonadiTag.isImmutable();
    const auto *attr = akonadiTag.attribute<Akonadi::TagAttribute>();
    if (attr) {
        if (!attr->iconName().isEmpty()) {
            tag->iconName = attr->iconName();
        }
        tag->inToolbar = attr->inToolbar();
        tag->shortcut = QKeySequence(attr->shortcut());
        tag->textColor = attr->textColor();
        tag->backgroundColor = attr->backgroundColor();
        if (!attr->font().isEmpty()) {
            QFont font;
            font.fromString(attr->font());
            tag->isBold = font.bold();
            tag->isItalic = font.italic();
        }
        tag->priority = attr->priority();
    }
    return tag;
}

Akonadi::Tag Tag::saveToAkonadi(Tag::SaveFlags saveFlags) const
{
    Akonadi::Tag tag = mTag;
    if (tag.gid().isEmpty()) {
        tag.setGid(QUuid::createUuid().toByteArray().mid(1, 36));
    }
    if (isImmutable) {
        tag.setType(Akonadi::Tag::PLAIN);
    } else {
        tag.setType(Akonadi::Tag::GENERIC);
    }
    auto *attr = tag.attribute<Akonadi::TagAttribute>(Akonadi::Tag::AddIfMissing);
    attr->setDisplayName(tagName);
    attr->setIconName(iconName);
    attr->setInToolbar(inToolbar);
    attr->setShortcut(shortcut.toString());
    attr->setPriority(priority);

    if (textColor.isValid() && (saveFlags & TextColor)) {
        attr->setTextColor(textColor);
    } else {
        attr->setTextColor(QColor());
    }

    if (backgroundColor.isValid() && (saveFlags & BackgroundColor)) {
        attr->setBackgroundColor(backgroundColor);
    } else {
        attr->setBackgroundColor(QColor());
    }

    if (saveFlags & Font) {
        QFont font = QGuiApplication::font();
        font.setBold(isBold);
        font.setItalic(isItalic);
        attr->setFont(font.toString());
    }

    tag.addAttribute(attr);
    return tag;
}

bool Tag::compare(const Tag::Ptr &tag1, const Tag::Ptr &tag2)
{
    if (tag1->priority < tag2->priority) {
        return true;
    } else if (tag1->priority == tag2->priority) {
        return tag1->tagName < tag2->tagName;
    } else {
        return false;
    }
}

bool Tag::compareName(const Tag::Ptr &tag1, const Tag::Ptr &tag2)
{
    return tag1->tagName < tag2->tagName;
}

bool Tag::operator==(const Tag &other) const
{
#if 0
    if (mTag.isValid()) {
        return id() == other.id();
    }
#endif
    return tagName == other.tagName && textColor == other.textColor && backgroundColor == other.backgroundColor && isBold == other.isBold
        && isItalic == other.isItalic && iconName == other.iconName && inToolbar == other.inToolbar && shortcut.toString() == other.shortcut.toString()
        && priority == other.priority;
}

bool Tag::operator!=(const Tag &other) const
{
    return !(*this == other);
}

qint64 Tag::id() const
{
    return mTag.id();
}

QString Tag::name() const
{
    return mTag.name();
}

Akonadi::Tag Tag::tag() const
{
    return mTag;
}
