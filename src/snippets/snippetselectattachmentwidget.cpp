/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "snippetselectattachmentwidget.h"
#include "snippetcustomfileattachmentnamedialog.h"
#include <KLocalizedString>
#include <QFileDialog>
#include <QPointer>
#include <QVBoxLayout>

using namespace MailCommon;
SnippetSelectAttachmentWidget::SnippetSelectAttachmentWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mEditor = new SnippetSelectorWidget(this);
    mEditor->setObjectName(QStringLiteral("editor"));
    mainLayout->addWidget(mEditor);
}

SnippetSelectAttachmentWidget::~SnippetSelectAttachmentWidget()
{
}

void SnippetSelectAttachmentWidget::setAttachments(const QStringList &lst)
{
    mEditor->setStringList(lst);
}

QStringList SnippetSelectAttachmentWidget::attachments() const
{
    return mEditor->stringList();
}

SnippetSelectorWidget::SnippetSelectorWidget(QWidget *parent)
    : PimCommon::SimpleStringListEditor(parent,
                                        static_cast<PimCommon::SimpleStringListEditor::ButtonCode>(PimCommon::SimpleStringListEditor::Add
                                                                                                   | PimCommon::SimpleStringListEditor::Remove
                                                                                                   | PimCommon::SimpleStringListEditor::Custom))
{
    setRemoveDialogLabel(i18n("Do you want to delete selected attachment?"));
}

SnippetSelectorWidget::~SnippetSelectorWidget()
{
}

void SnippetSelectorWidget::addNewEntry()
{
    const QStringList lst = QFileDialog::getOpenFileNames(this, i18n("Select Attachments"));
    if (!lst.isEmpty()) {
        appendStringList(lst);
    }
}

QString SnippetSelectorWidget::customEntry(const QString &text)
{
    QPointer<SnippetCustomFileAttachmentNameDialog> dlg = new SnippetCustomFileAttachmentNameDialog(this);
    dlg->setText(text);
    QString result;
    if (dlg->exec()) {
        result = dlg->result();
    }
    delete dlg;
    return result;
}
