/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "snippetattachmentwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "snippetselectattachmentdialog.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPointer>
#include <QToolButton>

using namespace MailCommon;

SnippetAttachmentWidget::SnippetAttachmentWidget(QWidget *parent)
    : QWidget(parent)
    , mLineEdit(new QLineEdit(this))
{
    auto layout = new QHBoxLayout(this);
    layout->setObjectName(QLatin1StringView("layout"));
    layout->setContentsMargins({});

    mLineEdit->setObjectName(QLatin1StringView("lineedit"));
    mLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Click on button for selecting attachment file"));
    layout->addWidget(mLineEdit);
    mLineEdit->setReadOnly(true);

    auto button = new QToolButton(this);
    button->setObjectName(QLatin1StringView("button"));
    button->setToolTip(i18nc("@info:tooltip", "Select Attachments"));
    button->setText(i18n("…"));
    layout->addWidget(button);
    connect(button, &QToolButton::clicked, this, &SnippetAttachmentWidget::slotSelectAttachment);
}

SnippetAttachmentWidget::~SnippetAttachmentWidget() = default;

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
    dlg->setAttachments(mLineEdit->text().split(u',', Qt::SkipEmptyParts));
    if (dlg->exec()) {
        mLineEdit->setText(dlg->attachments().join(u','));
        Q_EMIT wasChanged();
    }
    delete dlg;
}

#include "moc_snippetattachmentwidget.cpp"
