/*
   Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SNIPPETCUSTOMFILEATTACHMENTNAMEDIALOG_H
#define SNIPPETCUSTOMFILEATTACHMENTNAMEDIALOG_H

#include <QObject>
#include <QDialog>
#include "mailcommon_private_export.h"


namespace MailCommon {
class SnippetCustomFileAttachmentNameWidget;
class MAILCOMMON_TESTS_EXPORT SnippetCustomFileAttachmentNameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SnippetCustomFileAttachmentNameDialog(QWidget *parent = nullptr);
    ~SnippetCustomFileAttachmentNameDialog();

    Q_REQUIRED_RESULT QString result() const;

    void setText(const QString &str);

private:
    void readConfig();
    void writeConfig();
    SnippetCustomFileAttachmentNameWidget *mCustomWidget = nullptr;
};
}
#endif // SNIPPETCUSTOMFILEATTACHMENTNAMEDIALOG_H
