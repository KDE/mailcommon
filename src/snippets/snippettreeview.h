/***************************************************************************
 *   snippet feature from kdevelop/plugins/snippet/                        *
 *                                                                         *
 *   Copyright (C) 2007 by Robert Gruber                                   *
 *   rgruber@users.sourceforge.net                                         *
 *   Copyright (c) 2019 Montel Laurent <montel@kde.org>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MAILCOMMON_SNIPPETTREEVIEW_H
#define MAILCOMMON_SNIPPETTREEVIEW_H

#include <QTreeView>
#include "mailcommon_export.h"

class KActionCollection;

class QContextMenuEvent;

namespace MailCommon {
class SnippetsManager;
class SnippetInfo;
/**
 * @author Robert Gruber
 */
class MAILCOMMON_EXPORT SnippetTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit SnippetTreeView(KActionCollection *actionCollection, QWidget *parent = nullptr);
    ~SnippetTreeView() override;

    MailCommon::SnippetsManager *snippetsManager() const;

protected:
    void contextMenuEvent(QContextMenuEvent *) override;
    void dropEvent(QDropEvent *) override;

Q_SIGNALS:
    void insertSnippetInfo(const SnippetInfo &info);

private:
    MailCommon::SnippetsManager *mSnippetsManager = nullptr;
};
}
#endif