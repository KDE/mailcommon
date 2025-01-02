/*
  SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <QDialog>

namespace MailCommon
{
class AccountConfigOrderDialogPrivate;
class MailCommonSettings;
/**
 * @brief The AccountConfigOrderDialog class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT AccountConfigOrderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AccountConfigOrderDialog(MailCommon::MailCommonSettings *settings, QWidget *parent);
    ~AccountConfigOrderDialog() override;

private:
    MAILCOMMON_NO_EXPORT void slotOk();
    MAILCOMMON_NO_EXPORT void slotMoveUp();
    MAILCOMMON_NO_EXPORT void slotMoveDown();
    MAILCOMMON_NO_EXPORT void slotEnableControls();
    MAILCOMMON_NO_EXPORT void slotEnableAccountOrder(bool state);
    enum Type {
        IdentifierAccount = Qt::UserRole + 1,
    };

    MAILCOMMON_NO_EXPORT void writeConfig();
    MAILCOMMON_NO_EXPORT void readConfig();
    MAILCOMMON_NO_EXPORT void init();
    std::unique_ptr<AccountConfigOrderDialogPrivate> const d;
};
}
