/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SNIPPETSELECTATTACHMENTWIDGET_H
#define SNIPPETSELECTATTACHMENTWIDGET_H

#include <QWidget>
#include "mailcommon_private_export.h"
#include <PimCommon/SimpleStringListEditor>
namespace PimCommon {
class SimpleStringListEditor;
}
namespace MailCommon {
class MAILCOMMON_TESTS_EXPORT SnippetSelectorWidget : public PimCommon::SimpleStringListEditor
{
    Q_OBJECT
public:
    explicit SnippetSelectorWidget(QWidget *parent = nullptr);
    ~SnippetSelectorWidget();

    void addNewEntry() override;
    QString customEntry(const QString &text) override;
};

class MAILCOMMON_TESTS_EXPORT SnippetSelectAttachmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SnippetSelectAttachmentWidget(QWidget *parent = nullptr);
    ~SnippetSelectAttachmentWidget();

    void setAttachments(const QStringList &lst);
    Q_REQUIRED_RESULT QStringList attachments() const;
private:
    SnippetSelectorWidget *mEditor = nullptr;
};
}

#endif // SNIPPETSELECTATTACHMENTWIDGET_H
