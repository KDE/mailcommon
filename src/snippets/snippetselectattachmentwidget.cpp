/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "snippetselectattachmentwidget.h"
#include "snippetcustomfileattachmentnamedialog.h"
#include <PimCommon/SimpleStringListEditor>
#include <QVBoxLayout>
#include <QFileDialog>
#include <KLocalizedString>
#include <QPointer>

using namespace MailCommon;
SnippetSelectAttachmentWidget::SnippetSelectAttachmentWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
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
    : PimCommon::SimpleStringListEditor(parent, static_cast<PimCommon::SimpleStringListEditor::ButtonCode>(PimCommon::SimpleStringListEditor::Add
                                                                                                           |PimCommon::SimpleStringListEditor::Remove
                                                                                                           |PimCommon::SimpleStringListEditor::Custom))
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
    if (dlg->exec()) {
        return dlg->result();
    }
    return {};
}
