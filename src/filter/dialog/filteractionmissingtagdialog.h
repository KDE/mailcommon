/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONMISSINGTAGDIALOG_H
#define FILTERACTIONMISSINGTAGDIALOG_H

#include <QDialog>
class QListWidget;
namespace MailCommon
{
class FilterActionMissingTagDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingTagDialog(const QMap<QUrl, QString> &templateList,
                                          const QString &filtername,
                                          const QString &argsStr,
                                          QWidget *parent = nullptr);
    ~FilterActionMissingTagDialog();
    Q_REQUIRED_RESULT QString selectedTag() const;

private:
    void slotAddTag();
    void readConfig();
    void writeConfig();

    enum TypeData { UrlData = Qt::UserRole + 1 };
    QListWidget *mTagList = nullptr;
};
}
#endif // FILTERACTIONMISSINGTAGDIALOG_H
