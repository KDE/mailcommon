/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <QDialog>
namespace MailTransport
{
class TransportComboBox;
}

namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT FilterActionMissingTransportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingTransportDialog(const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingTransportDialog() override;
    [[nodiscard]] int selectedTransport() const;

private:
    MAILCOMMON_NO_EXPORT void writeConfig();
    MAILCOMMON_NO_EXPORT void readConfig();
    MailTransport::TransportComboBox *const mComboBoxTransport;
};
}
