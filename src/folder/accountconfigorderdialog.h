/*
  Copyright (c) 2012-2017 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ACCOUNTCONFIGORDERDIALOG_H
#define ACCOUNTCONFIGORDERDIALOG_H

#include <QDialog>
#include "mailcommon_export.h"

namespace MailCommon {
class AccountConfigOrderDialogPrivate;
class MailCommonSettings;
class MAILCOMMON_EXPORT AccountConfigOrderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AccountConfigOrderDialog(MailCommon::MailCommonSettings *settings, QWidget *parent);
    ~AccountConfigOrderDialog();

private:
    void slotOk();
    void slotMoveUp();
    void slotMoveDown();
    void slotEnableControls();
    void slotEnableAccountOrder(bool state);
    enum Type {
        IdentifierAccount = Qt::UserRole + 1
    };

    void writeConfig();
    void readConfig();
    void init();
    AccountConfigOrderDialogPrivate *const d;
};
}

#endif // ACCOUNTCONFIGORDERDIALOG_H
