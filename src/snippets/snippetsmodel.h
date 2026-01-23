/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
  SPDX-FileContributor: Tobias Koenig <tokoe@kdab.com>

  SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <QAbstractItemModel>
#include <QKeySequence>
namespace MailCommon
{
class SnippetItem;
/*!
 * \brief The SnippetsInfo struct
 * \author Laurent Montel <montel@kde.org>
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

/*!
 * \class MailCommon::SnippetsModel
 * \inmodule MailCommon
 * \inheaderfile MailCommon/SnippetsModel
 *
 * \brief The SnippetsModel class
 * \author Laurent Montel <montel@kde.org>
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

    /*!
     */
    static SnippetsModel *instance();

    /*!
     */
    explicit SnippetsModel(QObject *parent = nullptr);
    /*!
     */
    ~SnippetsModel() override;

    /*!
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    /*!
     */
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    /*!
     */
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    /*!
     */
    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    /*!
     */
    [[nodiscard]] QModelIndex parent(const QModelIndex &index) const override;

    /*!
     */
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     */
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     */
    [[nodiscard]] QStringList mimeTypes() const override;

    /*!
     */
    [[nodiscard]] QMimeData *mimeData(const QModelIndexList &indexes) const override;

    /*!
     */
    [[nodiscard]] bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    /*!
     */
    [[nodiscard]] Qt::DropActions supportedDropActions() const override;

    /*!
     */
    void save(const QString &filename = QString());
    /*!
     */
    void load(const QString &filename = QString());

    /*!
     */
    [[nodiscard]] QMap<QString, QString> savedVariables() const;
    /*!
     */
    void setSavedVariables(const QMap<QString, QString> &savedVariables);

    /*!
     */
    [[nodiscard]] QList<SnippetsInfo> snippetsInfo() const;

protected:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

Q_SIGNALS:
    /*!
     */
    void dndDone();
    /*!
     */
    void addNewDndSnippset(const QString &);
    /*!
     */
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
    MAILCOMMON_NO_EXPORT QModelIndex createGroup(const QString &groupName);
    MAILCOMMON_NO_EXPORT void createSnippet(const QModelIndex &groupIndex,
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
Q_DECLARE_TYPEINFO(MailCommon::SnippetsInfo, Q_RELOCATABLE_TYPE);
