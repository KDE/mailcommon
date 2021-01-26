/*

  SPDX-FileCopyrightText: 2011-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FILTERACTIONMISSINGFOLDERDIALOG_H
#define MAILCOMMON_FILTERACTIONMISSINGFOLDERDIALOG_H

#include "mailcommon_private_export.h"
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

class MAILCOMMON_TESTS_EXPORT FilterActionMissingFolderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingFolderDialog(const Akonadi::Collection::List &list,
                                             const QString &filtername = QString(),
                                             const QString &argStr = QString(),
                                             QWidget *parent = nullptr);
    ~FilterActionMissingFolderDialog();

    Q_REQUIRED_RESULT Akonadi::Collection selectedCollection() const;
    static Akonadi::Collection::List potentialCorrectFolders(const QString &path, bool &exactPath);

private:
    void slotCurrentItemChanged();
    void slotFolderChanged(const Akonadi::Collection &col);
    void slotDoubleItemClicked(QListWidgetItem *item);
    static void getPotentialFolders(const QAbstractItemModel *model, const QModelIndex &parentIndex, const QString &realPath, Akonadi::Collection::List &list);
    enum collectionEnum { IdentifyCollection = Qt::UserRole + 1 };

private:
    void writeConfig();
    void readConfig();
    MailCommon::FolderRequester *mFolderRequester = nullptr;
    QListWidget *mListwidget = nullptr;
    QPushButton *mOkButton = nullptr;
};

#endif /* FILTERACTIONMISSINGFOLDERDIALOG_H */
