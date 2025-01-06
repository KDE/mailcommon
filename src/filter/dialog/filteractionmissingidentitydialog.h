/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "mailcommon_private_export.h"
#include <QDialog>

namespace KIdentityManagementWidgets
{
class IdentityCombo;
}
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT FilterActionMissingIdentityDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingIdentityDialog(const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingIdentityDialog() override;
    [[nodiscard]] int selectedIdentity() const;

private:
    MAILCOMMON_NO_EXPORT void writeConfig();
    MAILCOMMON_NO_EXPORT void readConfig();
    KIdentityManagementWidgets::IdentityCombo *mComboBoxIdentity = nullptr;
};
}
