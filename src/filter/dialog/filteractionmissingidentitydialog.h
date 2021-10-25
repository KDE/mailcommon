/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

namespace KIdentityManagement
{
class IdentityCombo;
}
namespace MailCommon
{
class FilterActionMissingIdentityDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingIdentityDialog(const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingIdentityDialog() override;
    Q_REQUIRED_RESULT int selectedIdentity() const;

private:
    void writeConfig();
    void readConfig();
    KIdentityManagement::IdentityCombo *mComboBoxIdentity = nullptr;
};
}

