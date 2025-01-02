/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

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
    [[nodiscard]] QString text() const;
    void clear();

Q_SIGNALS:
    void wasChanged();

private:
    MAILCOMMON_NO_EXPORT void slotSelectAttachment();
    QLineEdit *const mLineEdit;
};
}
