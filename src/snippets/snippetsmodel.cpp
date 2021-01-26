/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
  SPDX-FileContributor: Tobias Koenig <tokoe@kdab.com>

  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "snippetsmodel.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <QDataStream>
#include <QMimeData>
#include <QStringList>

using namespace MailCommon;

class MailCommon::SnippetItem
{
public:
    explicit SnippetItem(bool isGroup = false, SnippetItem *parent = nullptr);
    ~SnippetItem();

    Q_REQUIRED_RESULT bool isGroup() const;

    void setName(const QString &name);
    Q_REQUIRED_RESULT QString name() const;

    void setText(const QString &text);
    Q_REQUIRED_RESULT QString text() const;

    void setKeyword(const QString &text);
    Q_REQUIRED_RESULT QString keyword() const;

    void setKeySequence(const QString &sequence);
    Q_REQUIRED_RESULT QString keySequence() const;

    void appendChild(SnippetItem *child);
    void removeChild(SnippetItem *child);
    SnippetItem *child(int row) const;
    Q_REQUIRED_RESULT int childCount() const;
    Q_REQUIRED_RESULT int row() const;
    SnippetItem *parent() const;

    Q_REQUIRED_RESULT QString subject() const;
    void setSubject(const QString &subject);

    Q_REQUIRED_RESULT QString to() const;
    void setTo(const QString &to);

    Q_REQUIRED_RESULT QString cc() const;
    void setCc(const QString &cc);

    Q_REQUIRED_RESULT QString bcc() const;
    void setBcc(const QString &bcc);

    Q_REQUIRED_RESULT QString attachment() const;
    void setAttachment(const QString &attachment);

private:
    QVector<SnippetItem *> mChildItems;
    SnippetItem *mParentItem = nullptr;

    bool mIsGroup = false;
    QString mName;
    QString mText;
    QString mKeySequence;
    QString mKeyword;
    QString mSubject;
    QString mTo;
    QString mCc;
    QString mBcc;
    QString mAttachment;
};

SnippetItem::SnippetItem(bool isGroup, SnippetItem *parent)
    : mParentItem(parent)
    , mIsGroup(isGroup)
{
}

SnippetItem::~SnippetItem()
{
    qDeleteAll(mChildItems);
    mChildItems.clear();
}

bool SnippetItem::isGroup() const
{
    return mIsGroup;
}

void SnippetItem::setName(const QString &name)
{
    mName = name;
}

QString SnippetItem::name() const
{
    return mName;
}

void SnippetItem::setText(const QString &text)
{
    mText = text;
}

QString SnippetItem::text() const
{
    return mText;
}

void SnippetItem::setKeyword(const QString &text)
{
    mKeyword = text;
}

QString SnippetItem::keyword() const
{
    return mKeyword;
}

void SnippetItem::setKeySequence(const QString &sequence)
{
    mKeySequence = sequence;
}

QString SnippetItem::keySequence() const
{
    return mKeySequence;
}

void SnippetItem::appendChild(SnippetItem *item)
{
    mChildItems.append(item);
}

void SnippetItem::removeChild(SnippetItem *item)
{
    mChildItems.removeAll(item);
    delete item;
}

SnippetItem *SnippetItem::child(int row) const
{
    return mChildItems.value(row);
}

int SnippetItem::childCount() const
{
    return mChildItems.count();
}

SnippetItem *SnippetItem::parent() const
{
    return mParentItem;
}

QString SnippetItem::subject() const
{
    return mSubject;
}

void SnippetItem::setSubject(const QString &subject)
{
    mSubject = subject;
}

QString SnippetItem::to() const
{
    return mTo;
}

void SnippetItem::setTo(const QString &to)
{
    mTo = to;
}

QString SnippetItem::cc() const
{
    return mCc;
}

void SnippetItem::setCc(const QString &cc)
{
    mCc = cc;
}

QString SnippetItem::bcc() const
{
    return mBcc;
}

void SnippetItem::setBcc(const QString &bcc)
{
    mBcc = bcc;
}

QString SnippetItem::attachment() const
{
    return mAttachment;
}

void SnippetItem::setAttachment(const QString &attachment)
{
    mAttachment = attachment;
}

int SnippetItem::row() const
{
    if (mParentItem) {
        return mParentItem->mChildItems.indexOf(const_cast<SnippetItem *>(this));
    }

    return 0;
}

SnippetsModel *SnippetsModel::instance()
{
    static SnippetsModel s_self;
    return &s_self;
}

SnippetsModel::SnippetsModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    mRootItem = new SnippetItem(true);
    load();
}

SnippetsModel::~SnippetsModel()
{
    delete mRootItem;
}

int SnippetsModel::columnCount(const QModelIndex &) const
{
    return 1;
}

bool SnippetsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    auto item = static_cast<SnippetItem *>(index.internalPointer());
    Q_ASSERT(item);

    switch (role) {
    case NameRole:
        item->setName(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    case TextRole:
        item->setText(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    case KeySequenceRole:
        item->setKeySequence(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    case KeywordRole:
        item->setKeyword(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    case SubjectRole:
        item->setSubject(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    case ToRole:
        item->setTo(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    case CcRole:
        item->setCc(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    case BccRole:
        item->setBcc(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    case AttachmentRole:
        item->setAttachment(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    }

    return false;
}

QVariant SnippetsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    auto item = static_cast<SnippetItem *>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
        return item->name();
    case IsGroupRole:
        return item->isGroup();
    case NameRole:
        return item->name();
    case TextRole:
        return item->text();
    case KeySequenceRole:
        return item->keySequence();
    case KeywordRole:
        return item->keyword();
    case SubjectRole:
        return item->subject();
    case ToRole:
        return item->to();
    case CcRole:
        return item->cc();
    case BccRole:
        return item->bcc();
    case AttachmentRole:
        return item->attachment();
    }

    return QVariant();
}

Qt::ItemFlags SnippetsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid()) {
        const SnippetItem *item = static_cast<SnippetItem *>(index.internalPointer());
        if (!item->isGroup()) {
            return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
        }
    }

    return Qt::ItemIsDropEnabled | defaultFlags;
}

QModelIndex SnippetsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    SnippetItem *parentItem = nullptr;

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<SnippetItem *>(parent.internalPointer());
    }

    SnippetItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex SnippetsModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    auto childItem = static_cast<SnippetItem *>(index.internalPointer());
    SnippetItem *parentItem = childItem->parent();

    if (parentItem == mRootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int SnippetsModel::rowCount(const QModelIndex &parent) const
{
    SnippetItem *parentItem = nullptr;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<SnippetItem *>(parent.internalPointer());
    }

    return parentItem->childCount();
}

bool SnippetsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    SnippetItem *parentItem = nullptr;

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<SnippetItem *>(parent.internalPointer());
    }

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        auto snippet = new SnippetItem(!parent.isValid(), parentItem);
        parentItem->appendChild(snippet);
    }
    endInsertRows();

    return true;
}

bool SnippetsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    SnippetItem *parentItem = nullptr;

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<SnippetItem *>(parent.internalPointer());
    }

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        parentItem->removeChild(parentItem->child(row));
    }
    endRemoveRows();

    return true;
}

QStringList SnippetsModel::mimeTypes() const
{
    return QStringList() << QStringLiteral("text/x-kmail-textsnippet") << QStringLiteral("text/plain");
}

QMimeData *SnippetsModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.isEmpty()) {
        return nullptr;
    }

    const QModelIndex index = indexes.first();

    auto item = static_cast<SnippetItem *>(index.internalPointer());
    if (item->isGroup()) {
        return nullptr;
    }

    auto mimeData = new QMimeData();

    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << index.parent().internalId() << item->name() << item->text() << item->keySequence() << item->keyword() << item->subject() << item->to()
           << item->cc() << item->bcc() << item->attachment();

    mimeData->setData(QStringLiteral("text/x-kmail-textsnippet"), encodedData);
    mimeData->setText(item->text());

    return mimeData;
}

bool SnippetsModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(row)

    if (action == Qt::IgnoreAction) {
        return true;
    }

    if (data->hasFormat(QStringLiteral("text/x-kmail-textsnippet"))) {
        if (!parent.isValid()) {
            return false;
        }

        if (column > 1) {
            return false;
        }

        auto item = static_cast<SnippetItem *>(parent.internalPointer());

        QByteArray encodedData = data->data(QStringLiteral("text/x-kmail-textsnippet"));
        QDataStream stream(&encodedData, QIODevice::ReadOnly);

        quintptr id;
        QString name;
        QString text;
        QString keySequence;
        QString keyword;
        QString subject;
        QString to;
        QString cc;
        QString bcc;
        QString attachment;
        stream >> id >> name >> text >> keySequence >> keyword >> subject >> to >> cc >> bcc >> attachment;
        if (parent.internalId() == id) {
            return false;
        }
        if (item->isGroup()) {
            insertRow(rowCount(parent), parent);

            const QModelIndex idx = index(rowCount(parent) - 1, 0, parent);

            setData(idx, name, SnippetsModel::NameRole);
            setData(idx, text, SnippetsModel::TextRole);
            setData(idx, keySequence, SnippetsModel::KeySequenceRole);
            setData(idx, keyword, SnippetsModel::KeywordRole);
            setData(idx, subject, SnippetsModel::SubjectRole);
            setData(idx, to, SnippetsModel::ToRole);
            setData(idx, cc, SnippetsModel::CcRole);
            setData(idx, bcc, SnippetsModel::BccRole);
            setData(idx, attachment, SnippetsModel::AttachmentRole);
            Q_EMIT dndDone();
            return true;
        } else {
            if (KMessageBox::Yes == KMessageBox::questionYesNo(nullptr, i18n("Do you want to update snippet?"), i18n("Update snippet"))) {
                item->setText(text);
                item->setSubject(subject);
                item->setTo(to);
                item->setCc(cc);
                item->setBcc(bcc);
                return true;
            }
            return false;
        }
    } else if (data->hasFormat(QStringLiteral("text/plain"))) {
        if (column > 1) {
            return false;
        }
        const QString encodedData = QString::fromUtf8(data->data(QStringLiteral("text/plain")));
        if (!parent.isValid()) {
            Q_EMIT addNewDndSnippset(encodedData);
            return false;
        }
        auto item = static_cast<SnippetItem *>(parent.internalPointer());

        if (item->isGroup()) {
            Q_EMIT addNewDndSnippset(encodedData);
        } else {
            if (KMessageBox::Yes == KMessageBox::questionYesNo(nullptr, i18n("Do you want to update snippet?"), i18n("Update snippet"))) {
                item->setText(encodedData);
            }
        }
        return false;
    }
    return false;
}

Qt::DropActions SnippetsModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QModelIndex SnippetsModel::createGroup(const QString &groupName)
{
    insertRow(rowCount(), QModelIndex());
    const QModelIndex groupIndex = index(rowCount() - 1, 0, QModelIndex());
    setData(groupIndex, groupName, SnippetsModel::NameRole);
    return groupIndex;
}

void SnippetsModel::load(const QString &filename)
{
    const KSharedConfig::Ptr config = KSharedConfig::openConfig(filename.isEmpty() ? QStringLiteral("kmailsnippetrc") : filename, KConfig::NoGlobals);

    const KConfigGroup snippetPartGroup = config->group("SnippetPart");

    const int groupCount = snippetPartGroup.readEntry("snippetGroupCount", 0);

    for (int i = 0; i < groupCount; ++i) {
        const KConfigGroup group = config->group(QStringLiteral("SnippetGroup_%1").arg(i));

        const QString groupName = group.readEntry("Name");

        // create group
        const QModelIndex groupIndex = createGroup(groupName);

        const int snippetCount = group.readEntry("snippetCount", 0);
        for (int j = 0; j < snippetCount; ++j) {
            const QString snippetName = group.readEntry(QStringLiteral("snippetName_%1").arg(j), QString());

            const QString snippetText = group.readEntry(QStringLiteral("snippetText_%1").arg(j), QString());

            const QString snippetKeySequence = group.readEntry(QStringLiteral("snippetKeySequence_%1").arg(j), QString());

            const QString snippetKeyword = group.readEntry(QStringLiteral("snippetKeyword_%1").arg(j), QString());

            const QString snippetSubject = group.readEntry(QStringLiteral("snippetSubject_%1").arg(j), QString());

            const QString to = group.readEntry(QStringLiteral("snippetTo_%1").arg(j), QString());

            const QString cc = group.readEntry(QStringLiteral("snippetCc_%1").arg(j), QString());

            const QString bcc = group.readEntry(QStringLiteral("snippetBcc_%1").arg(j), QString());

            const QString attachment = group.readEntry(QStringLiteral("snippetAttachment_%1").arg(j), QString());
            createSnippet(groupIndex, snippetName, snippetText, snippetKeySequence, snippetKeyword, snippetSubject, to, cc, bcc, attachment);
        }
    }

    const KConfigGroup group = config->group("SavedVariablesPart");
    const int variablesCount = group.readEntry("variablesCount", 0);

    for (int i = 0; i < variablesCount; ++i) {
        const QString variableKey = group.readEntry(QStringLiteral("variableName_%1").arg(i), QString());

        const QString variableValue = group.readEntry(QStringLiteral("variableValue_%1").arg(i), QString());

        mSavedVariables.insert(variableKey, variableValue);
    }
}

void SnippetsModel::createSnippet(const QModelIndex &groupIndex,
                                  const QString &snippetName,
                                  const QString &snippetText,
                                  const QString &snippetKeySequence,
                                  const QString &snippetKeyword,
                                  const QString &snippetSubject,
                                  const QString &to,
                                  const QString &cc,
                                  const QString &bcc,
                                  const QString &attachment)
{
    insertRow(rowCount(groupIndex), groupIndex);
    const QModelIndex modelIndex = index(rowCount(groupIndex) - 1, 0, groupIndex);

    setData(modelIndex, snippetName, SnippetsModel::NameRole);
    setData(modelIndex, snippetText, SnippetsModel::TextRole);
    setData(modelIndex, snippetKeySequence, SnippetsModel::KeySequenceRole);
    setData(modelIndex, snippetKeyword, SnippetsModel::KeywordRole);
    setData(modelIndex, snippetSubject, SnippetsModel::SubjectRole);

    setData(modelIndex, to, SnippetsModel::ToRole);
    setData(modelIndex, cc, SnippetsModel::CcRole);
    setData(modelIndex, bcc, SnippetsModel::BccRole);
    setData(modelIndex, attachment, SnippetsModel::AttachmentRole);

    Q_EMIT updateActionCollection(QString(), snippetName, QKeySequence::fromString(snippetKeySequence), snippetText, snippetSubject, to, cc, bcc, attachment);
}

void SnippetsModel::setSavedVariables(const QMap<QString, QString> &savedVariables)
{
    mSavedVariables = savedVariables;
}

QVector<SnippetsInfo> SnippetsModel::snippetsInfo() const
{
    QVector<SnippetsInfo> infos;
    const int groupCount = rowCount();

    for (int i = 0; i < groupCount; ++i) {
        const QModelIndex groupIndex = index(i, 0, QModelIndex());
        const int snippetCount = rowCount(groupIndex);
        for (int j = 0; j < snippetCount; ++j) {
            SnippetsInfo info;
            const QModelIndex modelIndex = index(j, 0, groupIndex);

            const QString snippetName = modelIndex.data(SnippetsModel::NameRole).toString();
            if (!snippetName.isEmpty()) {
                const QString snippetText = modelIndex.data(SnippetsModel::TextRole).toString();
                const QString snippetKeySequence = modelIndex.data(SnippetsModel::KeySequenceRole).toString();
                const QString snippetKeyword = modelIndex.data(SnippetsModel::KeywordRole).toString();
                const QString snippetSubject = modelIndex.data(SnippetsModel::SubjectRole).toString();
                const QString snippetTo = modelIndex.data(SnippetsModel::ToRole).toString();
                const QString snippetCc = modelIndex.data(SnippetsModel::CcRole).toString();
                const QString snippetBcc = modelIndex.data(SnippetsModel::BccRole).toString();
                const QString snippetAttachment = modelIndex.data(SnippetsModel::AttachmentRole).toString();
                info.text = snippetText;
                info.newName = snippetName;
                info.keyword = snippetKeyword;
                info.keySequence = QKeySequence::fromString(snippetKeySequence);
                info.subject = snippetSubject;
                info.to = snippetTo;
                info.cc = snippetCc;
                info.bcc = snippetBcc;
                info.attachment = snippetAttachment;
                infos.append(info);
            }
        }
    }
    return infos;
}

QMap<QString, QString> SnippetsModel::savedVariables() const
{
    return mSavedVariables;
}

void SnippetsModel::save(const QString &filename)
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig(filename.isEmpty() ? QStringLiteral("kmailsnippetrc") : filename, KConfig::NoGlobals);

    // clear everything
    const QStringList lst = config->groupList();
    for (const QString &group : lst) {
        config->deleteGroup(group);
    }

    // write number of snippet groups
    KConfigGroup group = config->group("SnippetPart");

    const int groupCount = rowCount();
    group.writeEntry("snippetGroupCount", groupCount);

    for (int i = 0; i < groupCount; ++i) {
        const QModelIndex groupIndex = index(i, 0, QModelIndex());
        const QString groupName = groupIndex.data(SnippetsModel::NameRole).toString();

        KConfigGroup group = config->group(QStringLiteral("SnippetGroup_%1").arg(i));
        group.writeEntry("Name", groupName);

        const int snippetCount = rowCount(groupIndex);

        group.writeEntry("snippetCount", snippetCount);
        for (int j = 0; j < snippetCount; ++j) {
            const QModelIndex modelIndex = index(j, 0, groupIndex);

            const QString snippetName = modelIndex.data(SnippetsModel::NameRole).toString();
            if (!snippetName.isEmpty()) {
                const QString snippetText = modelIndex.data(SnippetsModel::TextRole).toString();
                const QString snippetKeySequence = modelIndex.data(SnippetsModel::KeySequenceRole).toString();
                const QString snippetKeyword = modelIndex.data(SnippetsModel::KeywordRole).toString();
                const QString snippetSubject = modelIndex.data(SnippetsModel::SubjectRole).toString();
                const QString snippetTo = modelIndex.data(SnippetsModel::ToRole).toString();
                const QString snippetCc = modelIndex.data(SnippetsModel::CcRole).toString();
                const QString snippetBcc = modelIndex.data(SnippetsModel::BccRole).toString();
                const QString snippetAttachment = modelIndex.data(SnippetsModel::AttachmentRole).toString();

                group.writeEntry(QStringLiteral("snippetName_%1").arg(j), snippetName);
                if (!snippetText.isEmpty()) {
                    group.writeEntry(QStringLiteral("snippetText_%1").arg(j), snippetText);
                }
                if (!snippetKeySequence.isEmpty()) {
                    group.writeEntry(QStringLiteral("snippetKeySequence_%1").arg(j), snippetKeySequence);
                }
                if (!snippetKeyword.isEmpty()) {
                    group.writeEntry(QStringLiteral("snippetKeyword_%1").arg(j), snippetKeyword);
                }
                if (!snippetSubject.isEmpty()) {
                    group.writeEntry(QStringLiteral("snippetSubject_%1").arg(j), snippetSubject);
                }
                if (!snippetTo.isEmpty()) {
                    group.writeEntry(QStringLiteral("snippetTo_%1").arg(j), snippetTo);
                }
                if (!snippetCc.isEmpty()) {
                    group.writeEntry(QStringLiteral("snippetCc_%1").arg(j), snippetCc);
                }
                if (!snippetBcc.isEmpty()) {
                    group.writeEntry(QStringLiteral("snippetBcc_%1").arg(j), snippetBcc);
                }
                if (!snippetAttachment.isEmpty()) {
                    group.writeEntry(QStringLiteral("snippetAttachment_%1").arg(j), snippetAttachment);
                }
            }
        }
    }

    {
        KConfigGroup group = config->group("SavedVariablesPart");

        const int variablesCount = mSavedVariables.count();
        group.writeEntry("variablesCount", variablesCount);

        int counter = 0;
        QMap<QString, QString>::const_iterator it = mSavedVariables.cbegin();
        const QMap<QString, QString>::const_iterator itEnd = mSavedVariables.cend();
        for (; it != itEnd; ++it) {
            group.writeEntry(QStringLiteral("variableName_%1").arg(counter), it.key());
            group.writeEntry(QStringLiteral("variableValue_%1").arg(counter), it.value());
            counter++;
        }
    }
    config->sync();
}
