/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONMISSINGACCOUNTDIALOG_H
#define FILTERACTIONMISSINGACCOUNTDIALOG_H

#include <QDialog>
namespace MailCommon
{
class KMFilterAccountList;
class FilterActionMissingAccountDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingAccountDialog(const QStringList &lstAccount, const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingAccountDialog();
    Q_REQUIRED_RESULT QStringList selectedAccount() const;
    static bool allAccountExist(const QStringList &lst);

private:
    void readConfig();
    void writeConfig();
    MailCommon::KMFilterAccountList *mAccountList = nullptr;
};
}
#endif // FILTERACTIONMISSINGACCOUNTDIALOG_H
