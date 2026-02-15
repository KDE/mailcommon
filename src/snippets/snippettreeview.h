/*
  snippet feature from kdevelop/plugins/snippet/

  SPDX-FileCopyrightText: 2007 Robert Gruber <rgruber@users.sourceforge.net>
  SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <QTreeView>

class KActionCollection;

class QContextMenuEvent;

namespace MailCommon
{
class SnippetsManager;
struct SnippetInfo;
/*!
 * \class MailCommon::SnippetTreeView
 * \inmodule MailCommon
 * \inheaderfile MailCommon/SnippetTreeView
 *
 * \author Robert Gruber
 */
class MAILCOMMON_EXPORT SnippetTreeView : public QTreeView
{
    Q_OBJECT
public:
    /*!
     * Constructs a snippet tree view.
     *
     * \param actionCollection The action collection for managing context menu actions
     * \param parent The parent widget
     */
    explicit SnippetTreeView(KActionCollection *actionCollection, QWidget *parent = nullptr);
    /*!
     * Destroys the snippet tree view.
     */
    ~SnippetTreeView() override;

    /*!
     * Returns the snippets manager associated with this view.
     *
     * \return A pointer to the snippets manager
     */
    MailCommon::SnippetsManager *snippetsManager() const;

protected:
    /*!
     * Handles context menu events for the tree view.
     *
     * \param event The context menu event
     */
    void contextMenuEvent(QContextMenuEvent *event) override;

Q_SIGNALS:
    /*!
     * Emitted when a snippet should be inserted into the editor.
     *
     * \param info The snippet information to insert
     */
    void insertSnippetInfo(const MailCommon::SnippetInfo &info);

private:
    MailCommon::SnippetsManager *mSnippetsManager = nullptr;
};
}
