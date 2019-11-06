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

#include "snippetattachmentwidget.h"
#include "snippetselectattachmentdialog.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <KLocalizedString>
#include <QToolButton>
#include <QPointer>

using namespace MailCommon;

SnippetAttachmentWidget::SnippetAttachmentWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins(0, 0, 0, 0);

    mLineEdit = new QLineEdit(this);
    mLineEdit->setObjectName(QStringLiteral("lineedit"));
    layout->addWidget(mLineEdit);
    mLineEdit->setReadOnly(true);

    QToolButton *button = new QToolButton(this);
    button->setText(i18n("..."));
    button->setObjectName(QStringLiteral("button"));
    layout->addWidget(button);
    connect(button, &QToolButton::clicked, this, &SnippetAttachmentWidget::slotSelectAttachment);
}

SnippetAttachmentWidget::~SnippetAttachmentWidget()
{
}

void SnippetAttachmentWidget::setText(const QString &str)
{
    mLineEdit->setText(str);
}

QString SnippetAttachmentWidget::text() const
{
    return mLineEdit->text();
}

void SnippetAttachmentWidget::clear()
{
    mLineEdit->clear();
}

void SnippetAttachmentWidget::slotSelectAttachment()
{
    QPointer<MailCommon::SnippetSelectAttachmentDialog> dlg = new MailCommon::SnippetSelectAttachmentDialog(this);
    dlg->setAttachments(mLineEdit->text().split(QLatin1Char(',')));
    if (dlg->exec()) {
        mLineEdit->setText(dlg->attachments().join(QLatin1Char(',')));
        Q_EMIT wasChanged();
    }
    delete dlg;
}
