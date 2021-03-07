/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ACCOUNTCONFIGORDERDIALOG_H
#define ACCOUNTCONFIGORDERDIALOG_H

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
    void slotOk();
    void slotMoveUp();
    void slotMoveDown();
    void slotEnableControls();
    void slotEnableAccountOrder(bool state);
    enum Type { IdentifierAccount = Qt::UserRole + 1 };

    void writeConfig();
    void readConfig();
    void init();
    AccountConfigOrderDialogPrivate *const d;
};
}

#endif // ACCOUNTCONFIGORDERDIALOG_H
