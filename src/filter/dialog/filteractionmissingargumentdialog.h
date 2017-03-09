/*

  Copyright (c) 2011-2017 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAILCOMMON_FILTERACTIONMISSINGARGUMENTDIALOG_H
#define MAILCOMMON_FILTERACTIONMISSINGARGUMENTDIALOG_H

#include "mailcommon_export.h"

#include <Collection>

#include <QDialog>


class QAbstractItemModel;
class QListWidget;
class QListWidgetItem;
class QModelIndex;
class QPushButton;

namespace MailCommon
{
class FolderRequester;
}

class MAILCOMMON_EXPORT FilterActionMissingCollectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingCollectionDialog(const Akonadi::Collection::List &list,
            const QString &filtername = QString(),
            const QString &argStr = QString(),
            QWidget *parent = nullptr);
    ~FilterActionMissingCollectionDialog();

    Akonadi::Collection selectedCollection() const;
    static Akonadi::Collection::List potentialCorrectFolders(const QString &path,
            bool &exactPath);

private Q_SLOTS:
    void slotCurrentItemChanged();
    void slotFolderChanged(const Akonadi::Collection &col);
    void slotDoubleItemClicked(QListWidgetItem *item);

private:
    static void getPotentialFolders(const QAbstractItemModel *model,
                                    const QModelIndex &parentIndex,
                                    const QString &realPath,
                                    Akonadi::Collection::List &list);
    enum collectionEnum {
        IdentifyCollection = Qt::UserRole + 1
    };

private:
    void writeConfig();
    void readConfig();
    MailCommon::FolderRequester *mFolderRequester;
    QListWidget *mListwidget;
    QPushButton *mOkButton;
};

#endif /* FILTERACTIONMISSINGARGUMENTDIALOG_H */

