/*
  Copyright (C) 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.net,
    Author: Tobias Koenig <tokoe@kdab.com>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#ifndef MAILCOMMON_SNIPPETSMODEL_P_H
#define MAILCOMMON_SNIPPETSMODEL_P_H

#include <QAbstractItemModel>
#include <QKeySequence>
#include "mailcommon_export.h"
namespace MailCommon {
class SnippetItem;

struct MAILCOMMON_EXPORT SnippetsInfo
{
    QString newName;
    QKeySequence keySequence;
    QString text;
    QString keyword;
    QString subject;
};

class MAILCOMMON_EXPORT SnippetsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Role {
        IsGroupRole = Qt::UserRole + 1, ///< Returns whether the index represents a group
        NameRole,                       ///< The name of a snippet or group
        TextRole,                       ///< The text of a snippet
        KeySequenceRole,                ///< The key sequence to activate a snippet
        KeywordRole,                    ///< The keyword which will replace by snippet
        SubjectRole,                    ///< The subject of a snippet
    };

    static SnippetsModel *instance();

    explicit SnippetsModel(QObject *parent = nullptr);
    ~SnippetsModel() override;

    Q_REQUIRED_RESULT bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

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
    void updateActionCollection(const QString &oldName, const QString &newName, const QKeySequence &keySequence, const QString &text, const QString &subject);

private:
    QModelIndex createGroup(const QString &groupName);
    void createSnippet(const QModelIndex &groupIndex, const QString &snippetName, const QString &snippetText, const QString &snippetKeySequence, const QString &snippetKeyword, const QString &snippetSubject);
    SnippetItem *mRootItem = nullptr;
    QMap<QString, QString> mSavedVariables;
};
}
Q_DECLARE_TYPEINFO(MailCommon::SnippetsInfo, Q_MOVABLE_TYPE);

#endif
