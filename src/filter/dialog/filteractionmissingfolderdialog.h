/*

  SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <Akonadi/Collection>

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
    ~FilterActionMissingFolderDialog() override;

    [[nodiscard]] Akonadi::Collection selectedCollection() const;
    [[nodiscard]] static Akonadi::Collection::List potentialCorrectFolders(const QString &path, bool &exactPath);

private:
    MAILCOMMON_NO_EXPORT void slotCurrentItemChanged();
    MAILCOMMON_NO_EXPORT void slotFolderChanged(const Akonadi::Collection &col);
    MAILCOMMON_NO_EXPORT void slotDoubleItemClicked(QListWidgetItem *item);
    MAILCOMMON_NO_EXPORT static void
    getPotentialFolders(const QAbstractItemModel *model, const QModelIndex &parentIndex, const QString &realPath, Akonadi::Collection::List &list);
    MAILCOMMON_NO_EXPORT void writeConfig();
    MAILCOMMON_NO_EXPORT void readConfig();
    enum collectionEnum {
        IdentifyCollection = Qt::UserRole + 1
    };

    MailCommon::FolderRequester *const mFolderRequester;
    QListWidget *mListwidget = nullptr;
    QPushButton *mOkButton = nullptr;
};
