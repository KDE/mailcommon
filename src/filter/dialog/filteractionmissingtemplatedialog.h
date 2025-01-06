/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "mailcommon_private_export.h"
#include <QDialog>

class QComboBox;
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT FilterActionMissingTemplateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingTemplateDialog(const QStringList &templateList, const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingTemplateDialog() override;
    [[nodiscard]] QString selectedTemplate() const;

private:
    MAILCOMMON_NO_EXPORT void readConfig();
    MAILCOMMON_NO_EXPORT void writeConfig();
    QComboBox *const mComboBoxTemplate;
};
}
