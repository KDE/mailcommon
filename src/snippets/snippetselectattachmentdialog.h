/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "mailcommon_private_export.h"
#include <QDialog>
namespace MailCommon
{
class SnippetSelectAttachmentWidget;
class MAILCOMMON_TESTS_EXPORT SnippetSelectAttachmentDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SnippetSelectAttachmentDialog(QWidget *parent = nullptr);
    ~SnippetSelectAttachmentDialog() override;

    void setAttachments(const QStringList &lst);
    [[nodiscard]] QStringList attachments() const;

private:
    void writeConfig();
    void readConfig();
    SnippetSelectAttachmentWidget *const mAttachmentWidget;
};
}
