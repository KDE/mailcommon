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
 * \struct MailCommon::SnippetsInfo
 * \inmodule MailCommon
 * \inheaderfile MailCommon/SnippetsModel
 *
 * \brief The SnippetsInfo struct.
 *
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
 * \brief The SnippetsModel class.
 *
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
     * Returns the singleton instance of the snippets model.
     *
     * \return A pointer to the SnippetsModel instance
     */
    static SnippetsModel *instance();

    /*!
     * Constructs a snippets model.
     *
     * \a parent The parent object
     */
    explicit SnippetsModel(QObject *parent = nullptr);
    /*!
     * Destroys the snippets model.
     */
    ~SnippetsModel() override;

    /*!
     * Sets data for the given model index and role.
     *
     * \a index The model index to set data for
     * \a value The value to set
     * \a role The model role
     * \return True if the data was successfully set, false otherwise
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    /*!
     * Returns data for the given model index and role.
     *
     * \a index The model index to retrieve data for
     * \a role The model role
     * \return The data at the given index and role
     */
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    /*!
     * Returns the item flags for the given index.
     *
     * \a index The model index
     * \return The item flags
     */
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    /*!
     * Returns the model index for the given row, column, and parent.
     *
     * \a row The row number
     * \a column The column number
     * \a parent The parent model index
     * \return The model index
     */
    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * Returns the parent model index for the given index.
     *
     * \a index The model index
     * \return The parent model index
     */
    [[nodiscard]] QModelIndex parent(const QModelIndex &index) const override;

    /*!
     * Returns the number of rows in the model.
     *
     * \a parent The parent model index
     * \return The row count
     */
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * Returns the number of columns in the model.
     *
     * \a parent The parent model index
     * \return The column count
     */
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * Returns the list of supported MIME types.
     *
     * \return A string list of MIME types
     */
    [[nodiscard]] QStringList mimeTypes() const override;

    /*!
     * Returns MIME data for the given indexes.
     *
     * \a indexes The model indexes to encode
     * \return A pointer to the MIME data object
     */
    [[nodiscard]] QMimeData *mimeData(const QModelIndexList &indexes) const override;

    /*!
     * Handles dropping of MIME data onto the model.
     *
     * \a data The MIME data being dropped
     * \a action The drop action
     * \a row The destination row
     * \a column The destination column
     * \a parent The parent model index
     * \return True if the drop was successful, false otherwise
     */
    [[nodiscard]] bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    /*!
     * Returns the drop actions supported by the model.
     *
     * \return A combination of Qt::DropAction flags
     */
    [[nodiscard]] Qt::DropActions supportedDropActions() const override;

    /*!
     * Saves the snippets to a file.
     *
     * \a filename The filename to save to. If empty, uses the default location.
     */
    void save(const QString &filename = QString());
    /*!
     * Loads snippets from a file.
     *
     * \a filename The filename to load from. If empty, uses the default location.
     */
    void load(const QString &filename = QString());

    /*!
     * Returns the saved variables map.
     *
     * \return A map of variable names to values
     */
    [[nodiscard]] QMap<QString, QString> savedVariables() const;
    /*!
     * Sets the saved variables map.
     *
     * \a savedVariables The map of variable names to values
     */
    void setSavedVariables(const QMap<QString, QString> &savedVariables);

    /*!
     * Returns information about all snippets in the model.
     *
     * \return A list of SnippetsInfo objects
     */
    [[nodiscard]] QList<SnippetsInfo> snippetsInfo() const;

protected:
    /*!
     * Inserts new rows into the model.
     *
     * \a row The row to insert at
     * \a count The number of rows to insert
     * \a parent The parent model index
     * \return True if the rows were successfully inserted, false otherwise
     */
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    /*!
     * Removes rows from the model.
     *
     * \a row The row to remove
     * \a count The number of rows to remove
     * \a parent The parent model index
     * \return True if the rows were successfully removed, false otherwise
     */
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

Q_SIGNALS:
    /*!
     * Emitted when a drag-and-drop operation has completed.
     */
    void dndDone();
    /*!
     * Emitted when a new snippet has been added via drag-and-drop.
     *
     * \a text The text of the new snippet
     */
    void addNewDndSnippset(const QString &text);
    /*!
     * Emitted when an action collection needs to be updated for a snippet.
     *
     * \a oldName The old snippet name
     * \a newName The new snippet name
     * \a keySequence The keyboard shortcut
     * \a text The snippet text
     * \a subject The email subject
     * \a to The To email address
     * \a cc The CC email addresses
     * \a bcc The BCC email addresses
     * \a attachment The attachment path
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
