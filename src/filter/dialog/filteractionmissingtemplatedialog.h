/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

class QComboBox;
namespace MailCommon
{
class FilterActionMissingTemplateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingTemplateDialog(const QStringList &templateList, const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingTemplateDialog() override;
    [[nodiscard]] QString selectedTemplate() const;

private:
    void readConfig();
    void writeConfig();
    QComboBox *const mComboBoxTemplate;
};
}
