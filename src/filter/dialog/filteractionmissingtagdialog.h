/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <QDialog>
class QListWidget;
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT FilterActionMissingTagDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingTagDialog(const QMap<QUrl, QString> &templateList,
                                          const QString &filtername,
                                          const QString &argsStr,
                                          QWidget *parent = nullptr);
    ~FilterActionMissingTagDialog() override;
    [[nodiscard]] QString selectedTag() const;

private:
    void slotAddTag();
    void readConfig();
    void writeConfig();

    enum TypeData {
        UrlData = Qt::UserRole + 1
    };
    QListWidget *const mTagList;
};
}
