/*
  snippet feature from kdevelop/plugins/snippet/

  SPDX-FileCopyrightText: 2007 Robert Gruber <rgruber@users.sourceforge.net>
  SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

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

Q_SIGNALS:
    void insertSnippetInfo(const SnippetInfo &info);

private:
    MailCommon::SnippetsManager *mSnippetsManager = nullptr;
};
}
