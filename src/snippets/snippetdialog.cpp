/*
  snippet feature from kdevelop/plugins/snippet/

  SPDX-FileCopyrightText: 2007 Robert Gruber <rgruber@users.sourceforge.net>
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "snippetdialog.h"
#include "snippetwidget.h"

#include <KActionCollection>
#include <KConfigGroup>
#include <KPIMTextEdit/PlainTextEditor>
#include <KSharedConfig>
#include <MessageComposer/ConvertSnippetVariableMenu>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace MailCommon;
namespace
{
static const char myConfigGroupName[] = "SnippetDialog";
}
SnippetDialog::SnippetDialog(KActionCollection *actionCollection, bool inGroupMode, QWidget *parent)
    : QDialog(parent)
    , mActionCollection(actionCollection)
    , mInGroupMode(inGroupMode)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mSnippetWidget = new SnippetWidget(this);

    mainLayout->addWidget(mSnippetWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void SnippetDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
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

void SnippetDialog::setTo(const QString &keyword)
{
    mSnippetWidget->setTo(keyword);
}

QString SnippetDialog::to() const
{
    return mSnippetWidget->to();
}

void SnippetDialog::setCc(const QString &keyword)
{
    mSnippetWidget->setCc(keyword);
}

QString SnippetDialog::cc() const
{
    return mSnippetWidget->cc();
}

void SnippetDialog::setBcc(const QString &keyword)
{
    mSnippetWidget->setBcc(keyword);
}

QString SnippetDialog::bcc() const
{
    return mSnippetWidget->bcc();
}

void SnippetDialog::setAttachment(const QString &keyword)
{
    mSnippetWidget->setAttachment(keyword);
}

QString SnippetDialog::attachment() const
{
    return mSnippetWidget->attachment();
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
