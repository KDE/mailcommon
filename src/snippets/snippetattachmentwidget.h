/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SNIPPETATTACHMENTWIDGET_H
#define SNIPPETATTACHMENTWIDGET_H

#include "mailcommon_private_export.h"
#include <QWidget>

class QLineEdit;

namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT SnippetAttachmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SnippetAttachmentWidget(QWidget *parent = nullptr);
    ~SnippetAttachmentWidget() override;
    void setText(const QString &str);
    Q_REQUIRED_RESULT QString text() const;
    void clear();

Q_SIGNALS:
    void wasChanged();

private:
    void slotSelectAttachment();
    QLineEdit *mLineEdit = nullptr;
};
}

#endif // SNIPPETATTACHMENTWIDGET_H
