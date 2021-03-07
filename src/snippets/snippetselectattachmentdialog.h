/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SNIPPETSELECTATTACHMENTDIALOG_H
#define SNIPPETSELECTATTACHMENTDIALOG_H

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
    Q_REQUIRED_RESULT QStringList attachments() const;

private:
    void writeConfig();
    void readConfig();
    SnippetSelectAttachmentWidget *mAttachmentWidget = nullptr;
};
}

#endif // SNIPPETSELECTATTACHMENTDIALOG_H
