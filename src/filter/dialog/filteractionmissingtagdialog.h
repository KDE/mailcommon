/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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

#ifndef FILTERACTIONMISSINGTAGDIALOG_H
#define FILTERACTIONMISSINGTAGDIALOG_H

#include <QDialog>
class QListWidget;
namespace MailCommon {
class FilterActionMissingTagDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingTagDialog(const QMap<QUrl, QString> &templateList, const QString &filtername, const QString &argsStr, QWidget *parent = nullptr);
    ~FilterActionMissingTagDialog();
    Q_REQUIRED_RESULT QString selectedTag() const;

private:
    void slotAddTag();
    void readConfig();
    void writeConfig();

    enum TypeData {
        UrlData = Qt::UserRole + 1
    };
    QListWidget *mTagList = nullptr;
};
}
#endif // FILTERACTIONMISSINGTAGDIALOG_H
