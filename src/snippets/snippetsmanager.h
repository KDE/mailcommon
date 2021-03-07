/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  SPDX-FileContributor: Tobias Koenig <tokoe@kdab.com>

  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MAILCOMMON_SNIPPETSMANAGER_H
#define MAILCOMMON_SNIPPETSMANAGER_H

#include "mailcommon_export.h"

#include <QObject>

class KActionCollection;

class QAbstractItemModel;
class QAction;
class QItemSelectionModel;
namespace MailCommon
{
/**
 * @brief The SnippetInfo struct
 * @author Laurent Montel <montel@kde.org>
 */
struct MAILCOMMON_EXPORT SnippetInfo {
    SnippetInfo(const QString &_subject, const QString &_text, const QString &_to, const QString &_cc, const QString &_bcc, const QString &_attachment)
        : subject(_subject)
        , text(_text)
        , to(_to)
        , cc(_cc)
        , bcc(_bcc)
        , attachment(_attachment)
    {
    }

    QString subject;
    QString text;
    QString to;
    QString cc;
    QString bcc;
    QString attachment;
};
/**
 * @brief The SnippetsManager class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT SnippetsManager : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates a new snippets manager.
     *
     * @param actionCollection The action collection where the manager will
     *                         register the snippet shortcuts at.
     * @param parent The parent object.
     * @param widget The widget.
     */
    explicit SnippetsManager(KActionCollection *actionCollection, QObject *parent = nullptr, QWidget *widget = nullptr);

    /**
     * Destroys the snippets manager.
     */
    ~SnippetsManager() override;
    /**
     * Returns the model that represents the snippets.
     */
    QAbstractItemModel *model() const;

    /**
     * Returns the selection model that is used by the manager to select the
     * snippet or snippet group to work on.
     */
    QItemSelectionModel *selectionModel() const;

    /**
     * Returns the action that handles adding new snippets.
     */
    QAction *addSnippetAction() const;

    /**
     * Returns the action that handles editing the currently selected snippet.
     */
    QAction *editSnippetAction() const;

    /**
     * Returns the action that handles deleting the currently selected snippet.
     */
    QAction *deleteSnippetAction() const;

    /**
     * Returns the action that handles adding new snippet groups.
     */
    QAction *addSnippetGroupAction() const;

    /**
     * Returns the action that handles editing the currently selected snippet group.
     */
    QAction *editSnippetGroupAction() const;

    /**
     * Returns the action that handles deleting the currently selected snippet group.
     */
    QAction *deleteSnippetGroupAction() const;

    /**
     * Returns the action that handles inserting a snippet into the editor.
     */
    QAction *insertSnippetAction() const;

    /**
     * Returns whether the currently selected item is a snippet group.
     */
    Q_REQUIRED_RESULT bool snippetGroupSelected() const;

    /**
     * Returns the name of the currently selected snippet or snippet group.
     */
    Q_REQUIRED_RESULT QString selectedName() const;

Q_SIGNALS:
    void insertSnippet();
    void insertSnippetInfo(const SnippetInfo &info);

private:
    //@cond PRIVATE
    class Private;
    Private *const d;
    //@endcond
};
}

#endif
