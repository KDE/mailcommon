/*
   SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>
namespace MailCommon
{
class KMFilterAccountList;
class FilterActionMissingAccountDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingAccountDialog(const QStringList &lstAccount, const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingAccountDialog() override;
    [[nodiscard]] QStringList selectedAccount() const;
    [[nodiscard]] static bool allAccountExist(const QStringList &lst);

private:
    void readConfig();
    void writeConfig();
    MailCommon::KMFilterAccountList *const mAccountList;
};
}
