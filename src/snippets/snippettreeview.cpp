/*
  snippet feature from kdevelop/plugins/snippet/

  SPDX-FileCopyrightText: 2007 Robert Gruber <rgruber@users.sourceforge.net>
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "snippettreeview.h"
#include "snippetsmanager.h"

#include <KActionCollection>
#include <KLocalizedString>

#include <QContextMenuEvent>
#include <QHeaderView>
#include <QMenu>
using namespace MailCommon;
SnippetTreeView::SnippetTreeView(KActionCollection *actionCollection, QWidget *parent)
    : QTreeView(parent)
{
    header()->hide();
    setAcceptDrops(true);
    setDragEnabled(true);
    setRootIsDecorated(true);
    setAlternatingRowColors(true);
    mSnippetsManager = new MailCommon::SnippetsManager(actionCollection, this, this);
    connect(mSnippetsManager, &MailCommon::SnippetsManager::insertSnippetInfo, this, &SnippetTreeView::insertSnippetInfo);

    setModel(mSnippetsManager->model());
    setSelectionModel(mSnippetsManager->selectionModel());

    connect(this, &QAbstractItemView::activated, mSnippetsManager->insertSnippetAction(), &QAction::trigger);
    connect(mSnippetsManager->model(), &QAbstractItemModel::rowsInserted, this, &QTreeView::expandAll);
    connect(mSnippetsManager->model(), &QAbstractItemModel::rowsRemoved, this, &QTreeView::expandAll);

    expandAll();
}

SnippetTreeView::~SnippetTreeView()
{
}

void SnippetTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu popup;

    const bool itemSelected = mSnippetsManager->selectionModel()->hasSelection();

    bool canAddSnippet = true;
    if (itemSelected) {
        popup.setTitle(mSnippetsManager->selectedName());
        if (mSnippetsManager->snippetGroupSelected()) {
            popup.addAction(mSnippetsManager->editSnippetGroupAction());
            popup.addAction(mSnippetsManager->deleteSnippetGroupAction());
        } else {
            canAddSnippet = false; // subsnippets are not permitted
            popup.addAction(mSnippetsManager->addSnippetAction());
            popup.addAction(mSnippetsManager->editSnippetAction());
            popup.addAction(mSnippetsManager->deleteSnippetAction());
            popup.addAction(mSnippetsManager->insertSnippetAction());
        }
        popup.addSeparator();
    } else {
        popup.setTitle(i18n("Text Snippets"));
    }
    if (canAddSnippet) {
        popup.addAction(mSnippetsManager->addSnippetAction());
    }
    popup.addAction(mSnippetsManager->addSnippetGroupAction());

    popup.exec(event->globalPos());
}

void SnippetTreeView::dropEvent(QDropEvent *event)
{
    if (event->source() == this) {
        event->setDropAction(Qt::MoveAction);
    }
    QTreeView::dropEvent(event);
}

MailCommon::SnippetsManager *SnippetTreeView::snippetsManager() const
{
    return mSnippetsManager;
}
