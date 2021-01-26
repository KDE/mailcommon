/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "snippetattachmentwidget.h"
#include "snippetselectattachmentdialog.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPointer>
#include <QToolButton>

using namespace MailCommon;

SnippetAttachmentWidget::SnippetAttachmentWidget(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QHBoxLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins(0, 0, 0, 0);

    mLineEdit = new QLineEdit(this);
    mLineEdit->setObjectName(QStringLiteral("lineedit"));
    mLineEdit->setPlaceholderText(i18n("Click on button for selecting attachment file"));
    layout->addWidget(mLineEdit);
    mLineEdit->setReadOnly(true);

    auto button = new QToolButton(this);
    button->setObjectName(QStringLiteral("button"));
    button->setToolTip(i18n("Select Attachments"));
    button->setText(i18n("..."));
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
    dlg->setAttachments(mLineEdit->text().split(QLatin1Char(','), Qt::SkipEmptyParts));
    if (dlg->exec()) {
        mLineEdit->setText(dlg->attachments().join(QLatin1Char(',')));
        Q_EMIT wasChanged();
    }
    delete dlg;
}
