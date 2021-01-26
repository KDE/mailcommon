/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
  SPDX-FileContributor: Tobias Koenig <tokoe@kdab.com>

  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MAILCOMMON_SNIPPETSMODEL_P_H
#define MAILCOMMON_SNIPPETSMODEL_P_H

#include "mailcommon_export.h"
#include <QAbstractItemModel>
#include <QKeySequence>
namespace MailCommon
{
class SnippetItem;
/**
 * @brief The SnippetsInfo struct
 * @author Laurent Montel <montel@kde.org>
 */
struct MAILCOMMON_EXPORT SnippetsInfo {
    QString newName;
    QKeySequence keySequence;
    QString text;
    QString keyword;
    QString subject;
    QString to;
    QString cc;
    QString bcc;
    QString attachment;
};

/**
 * @brief The SnippetsModel class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT SnippetsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Role {
        IsGroupRole = Qt::UserRole + 1, ///< Returns whether the index represents a group
        NameRole, ///< The name of a snippet or group
        TextRole, ///< The text of a snippet
        KeySequenceRole, ///< The key sequence to activate a snippet
        KeywordRole, ///< The keyword which will replace by snippet
        SubjectRole, ///< The subject of a snippet
        ToRole, ///< The To of a snippet
        CcRole, ///< The Cc of a snippet
        BccRole, ///< The Cc of a snippet
        AttachmentRole, ///< The Attachment of a snippet
    };

    static SnippetsModel *instance();

    explicit SnippetsModel(QObject *parent = nullptr);
    ~SnippetsModel() override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    Q_REQUIRED_RESULT Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_REQUIRED_RESULT QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    Q_REQUIRED_RESULT QModelIndex parent(const QModelIndex &index) const override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_REQUIRED_RESULT QStringList mimeTypes() const override;

    Q_REQUIRED_RESULT QMimeData *mimeData(const QModelIndexList &indexes) const override;

    Q_REQUIRED_RESULT bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    Q_REQUIRED_RESULT Qt::DropActions supportedDropActions() const override;

    void save(const QString &filename = QString());
    void load(const QString &filename = QString());

    Q_REQUIRED_RESULT QMap<QString, QString> savedVariables() const;
    void setSavedVariables(const QMap<QString, QString> &savedVariables);

    Q_REQUIRED_RESULT QVector<SnippetsInfo> snippetsInfo() const;

protected:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

Q_SIGNALS:
    void dndDone();
    void addNewDndSnippset(const QString &);
    void updateActionCollection(const QString &oldName,
                                const QString &newName,
                                const QKeySequence &keySequence,
                                const QString &text,
                                const QString &subject,
                                const QString &to,
                                const QString &cc,
                                const QString &bcc,
                                const QString &attachment);

private:
    QModelIndex createGroup(const QString &groupName);
    void createSnippet(const QModelIndex &groupIndex,
                       const QString &snippetName,
                       const QString &snippetText,
                       const QString &snippetKeySequence,
                       const QString &snippetKeyword,
                       const QString &snippetSubject,
                       const QString &to,
                       const QString &cc,
                       const QString &bcc,
                       const QString &attachment);
    SnippetItem *mRootItem = nullptr;
    QMap<QString, QString> mSavedVariables;
};
}
Q_DECLARE_TYPEINFO(MailCommon::SnippetsInfo, Q_MOVABLE_TYPE);

#endif
