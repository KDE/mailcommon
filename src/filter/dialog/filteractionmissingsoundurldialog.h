/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>
class KUrlRequester;
namespace MailCommon
{
class FilterActionMissingSoundUrlDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingSoundUrlDialog(const QString &filtername, const QString &argStr, QWidget *parent = nullptr);
    ~FilterActionMissingSoundUrlDialog() override;
    [[nodiscard]] QString soundUrl() const;

private:
    void readConfig();
    void writeConfig();
    KUrlRequester *const mUrlWidget;
};
}
