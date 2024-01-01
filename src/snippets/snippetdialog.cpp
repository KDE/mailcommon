/*
  snippet feature from kdevelop/plugins/snippet/

  SPDX-FileCopyrightText: 2007 Robert Gruber <rgruber@users.sourceforge.net>
  SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "snippetdialog.h"
#include "snippetwidget.h"

#include <KActionCollection>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <MessageComposer/ConvertSnippetVariableMenu>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

using namespace MailCommon;
namespace
{
static const char mySnippetDialogConfigGroupName[] = "SnippetDialog";
}
SnippetDialog::SnippetDialog(KActionCollection *actionCollection, bool inGroupMode, QWidget *parent)
    : QDialog(parent)
    , mSnippetWidget(new SnippetWidget(this))
    , mInGroupMode(inGroupMode)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QLatin1StringView("mainLayout"));

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

void SnippetDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(300, 350));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(mySnippetDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SnippetDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(mySnippetDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
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

#include "moc_snippetdialog.cpp"
