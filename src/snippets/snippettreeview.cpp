/*
  snippet feature from kdevelop/plugins/snippet/

  SPDX-FileCopyrightText: 2007 Robert Gruber <rgruber@users.sourceforge.net>
  SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

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
    setDragDropMode(DragDrop);
    setRootIsDecorated(true);
    setAlternatingRowColors(true);
    setDefaultDropAction(Qt::MoveAction); // allow to reorder without having to hold Shift
    mSnippetsManager = new MailCommon::SnippetsManager(actionCollection, this, this);
    connect(mSnippetsManager, &MailCommon::SnippetsManager::insertSnippetInfo, this, &SnippetTreeView::insertSnippetInfo);

    setModel(mSnippetsManager->model());
    setSelectionModel(mSnippetsManager->selectionModel());

    connect(this, &QAbstractItemView::activated, mSnippetsManager->insertSnippetAction(), &QAction::trigger);
    connect(mSnippetsManager->model(), &QAbstractItemModel::rowsInserted, this, &QTreeView::expandAll);
    connect(mSnippetsManager->model(), &QAbstractItemModel::rowsRemoved, this, &QTreeView::expandAll);

    expandAll();
}

SnippetTreeView::~SnippetTreeView() = default;

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

MailCommon::SnippetsManager *SnippetTreeView::snippetsManager() const
{
    return mSnippetsManager;
}

#include "moc_snippettreeview.cpp"
