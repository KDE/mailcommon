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

#ifndef SNIPPETWIDGET_H
#define SNIPPETWIDGET_H

#include <QWidget>
#include <QAbstractItemModel>
#include "mailcommon_export.h"
class KActionCollection;
class SnippetWidgetPrivate;
namespace MailCommon {
class MAILCOMMON_EXPORT SnippetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SnippetWidget(QWidget *parent = nullptr);
    ~SnippetWidget();

    void setName(const QString &name);
    Q_REQUIRED_RESULT QString name() const;

    void setText(const QString &text);
    Q_REQUIRED_RESULT QString text() const;

    void setKeySequence(const QKeySequence &sequence);
    Q_REQUIRED_RESULT QKeySequence keySequence() const;

    void setKeyword(const QString &keyword);
    Q_REQUIRED_RESULT QString keyword() const;

    void setGroupModel(QAbstractItemModel *model);
    void setGroupIndex(const QModelIndex &index);

    Q_REQUIRED_RESULT QModelIndex groupIndex() const;
    Q_REQUIRED_RESULT bool snippetIsValid() const;

    void setCheckActionCollections(const QList<KActionCollection *> &lst);

    void setGroupSelected(bool b);

    Q_REQUIRED_RESULT bool isGroupSelected() const;
    void clear();

    Q_REQUIRED_RESULT bool wasChanged() const;
    void setWasChanged(bool b);

    void setSubject(const QString &text);
    Q_REQUIRED_RESULT QString subject() const;

Q_SIGNALS:
    void textChanged(const QString &str);
    void groupChanged(int index);
private:
    SnippetWidgetPrivate *const d;
};
}

#endif // SNIPPETWIDGET_H
