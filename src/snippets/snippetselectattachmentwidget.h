/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "mailcommon_private_export.h"
#include <PimCommon/SimpleStringListEditor>
#include <QWidget>
namespace PimCommon
{
class SimpleStringListEditor;
}
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT SnippetSelectorWidget : public PimCommon::SimpleStringListEditor
{
    Q_OBJECT
public:
    explicit SnippetSelectorWidget(QWidget *parent = nullptr);
    ~SnippetSelectorWidget() override;

    void addNewEntry() override;
    QString customEntry(const QString &text) override;
};

class MAILCOMMON_TESTS_EXPORT SnippetSelectAttachmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SnippetSelectAttachmentWidget(QWidget *parent = nullptr);
    ~SnippetSelectAttachmentWidget() override;

    void setAttachments(const QStringList &lst);
    Q_REQUIRED_RESULT QStringList attachments() const;

private:
    SnippetSelectorWidget *const mEditor;
};
}
