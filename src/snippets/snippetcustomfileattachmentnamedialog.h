/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SNIPPETCUSTOMFILEATTACHMENTNAMEDIALOG_H
#define SNIPPETCUSTOMFILEATTACHMENTNAMEDIALOG_H

#include <QDialog>
#include "mailcommon_private_export.h"

namespace MailCommon {
class SnippetCustomFileAttachmentNameWidget;
class MAILCOMMON_TESTS_EXPORT SnippetCustomFileAttachmentNameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SnippetCustomFileAttachmentNameDialog(QWidget *parent = nullptr);
    ~SnippetCustomFileAttachmentNameDialog();

    Q_REQUIRED_RESULT QString result() const;

    void setText(const QString &str);

private:
    void readConfig();
    void writeConfig();
    SnippetCustomFileAttachmentNameWidget *mCustomWidget = nullptr;
};
}
#endif // SNIPPETCUSTOMFILEATTACHMENTNAMEDIALOG_H
