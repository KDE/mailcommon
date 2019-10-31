/***************************************************************************
 *   snippet feature from kdevelop/plugins/snippet/                        *
 *                                                                         *
 *   Copyright (C) 2007 by Robert Gruber                                   *
 *   rgruber@users.sourceforge.net                                         *
 *   Copyright (c) 2019 Montel Laurent <montel@kde.org>                    *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "snippetdialog.h"
#include "snippetwidget.h"

#include <MessageComposer/ConvertSnippetVariableMenu>
#include <KPIMTextEdit/PlainTextEditor>
#include <kactioncollection.h>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <KConfigGroup>
#include <KSharedConfig>

using namespace MailCommon;
namespace {
static const char myConfigGroupName[] = "SnippetDialog";
}
SnippetDialog::SnippetDialog(KActionCollection *actionCollection, bool inGroupMode, QWidget *parent)
    : QDialog(parent)
    , mActionCollection(actionCollection)
    , mInGroupMode(inGroupMode)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mSnippetWidget = new SnippetWidget(this);

    mainLayout->addWidget(mSnippetWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setDefault(true);
    mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SnippetDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SnippetDialog::reject);
    mainLayout->addWidget(buttonBox);

    mSnippetWidget->setCheckActionCollections(QList<KActionCollection *>() << actionCollection);
    mOkButton->setEnabled(false);

    connect(mSnippetWidget, &MailCommon::SnippetWidget::textChanged, this, &SnippetDialog::slotTextChanged);
    connect(mSnippetWidget, &MailCommon::SnippetWidget::groupChanged, this, &SnippetDialog::slotGroupChanged);

    mSnippetWidget->setGroupSelected(mInGroupMode);
    if (!mInGroupMode) {
        readConfig();
    }

}

SnippetDialog::~SnippetDialog()
{
    if (!mInGroupMode) {
        writeConfig();
    }
}

void SnippetDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void SnippetDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 350));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SnippetDialog::slotGroupChanged()
{
    mOkButton->setEnabled(snippetIsValid());
}

void SnippetDialog::setName(const QString &name)
{
    mSnippetWidget->setName(name);
}

QString SnippetDialog::name() const
{
    return mSnippetWidget->name();
}

void SnippetDialog::setText(const QString &text)
{
    mSnippetWidget->setText(text);
}

QString SnippetDialog::text() const
{
    return mSnippetWidget->text();
}

void SnippetDialog::setSubject(const QString &text)
{
    mSnippetWidget->setSubject(text);
}

QString SnippetDialog::subject() const
{
    return mSnippetWidget->subject();
}

void SnippetDialog::setKeySequence(const QKeySequence &sequence)
{
    mSnippetWidget->setKeySequence(sequence);
}

QKeySequence SnippetDialog::keySequence() const
{
    return mSnippetWidget->keySequence();
}

void SnippetDialog::setKeyword(const QString &keyword)
{
    mSnippetWidget->setKeyword(keyword);
}

QString SnippetDialog::keyword() const
{
    return mSnippetWidget->keyword();
}

void SnippetDialog::setGroupModel(QAbstractItemModel *model)
{
    mSnippetWidget->setGroupModel(model);
}

void SnippetDialog::setGroupIndex(const QModelIndex &index)
{
    mSnippetWidget->setGroupIndex(index);
}

QModelIndex SnippetDialog::groupIndex() const
{
    return mSnippetWidget->groupIndex();
}

void SnippetDialog::slotTextChanged()
{
    mOkButton->setEnabled(snippetIsValid());
}

bool SnippetDialog::snippetIsValid() const
{
    return mSnippetWidget->snippetIsValid();
}
