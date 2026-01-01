/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <QDialog>
namespace MailCommon
{
class KMFilterAccountList;
class MAILCOMMON_TESTS_EXPORT FilterActionMissingAccountDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingAccountDialog(const QStringList &accounts, const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingAccountDialog() override;
    [[nodiscard]] QStringList selectedAccounts() const;
    [[nodiscard]] static bool allAccountsExist(const QStringList &lst);

private:
    MAILCOMMON_NO_EXPORT void readConfig();
    MAILCOMMON_NO_EXPORT void writeConfig();
    MailCommon::KMFilterAccountList *const mAccountsSelector;
};
}
