/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONMISSINGTRANSPORTDIALOG_H
#define FILTERACTIONMISSINGTRANSPORTDIALOG_H

#include <QDialog>
namespace MailTransport
{
class TransportComboBox;
}

namespace MailCommon
{
class FilterActionMissingTransportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingTransportDialog(const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingTransportDialog();
    Q_REQUIRED_RESULT int selectedTransport() const;

private:
    void writeConfig();
    void readConfig();
    MailTransport::TransportComboBox *mComboBoxTransport = nullptr;
};
}

#endif // FILTERACTIONMISSINGTRANSPORTDIALOG_H
