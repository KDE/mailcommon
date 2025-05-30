/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <QDialog>

namespace MailCommon
{
class SnippetCustomFileAttachmentNameWidget;
class MAILCOMMON_TESTS_EXPORT SnippetCustomFileAttachmentNameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SnippetCustomFileAttachmentNameDialog(QWidget *parent = nullptr);
    ~SnippetCustomFileAttachmentNameDialog() override;

    [[nodiscard]] QString result() const;

    void setText(const QString &str);

private:
    MAILCOMMON_NO_EXPORT void readConfig();
    MAILCOMMON_NO_EXPORT void writeConfig();
    SnippetCustomFileAttachmentNameWidget *const mCustomWidget;
};
}
