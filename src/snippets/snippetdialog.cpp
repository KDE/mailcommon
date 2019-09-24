/***************************************************************************
 *   snippet feature from kdevelop/plugins/snippet/                        *
 *                                                                         *
 *   Copyright (C) 2007 by Robert Gruber                                   *
 *   rgruber@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "snippetdialog.h"

#include "ui_snippetdialog.h"
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
{
    mUi = new Ui::SnippetDialog;
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mainWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setDefault(true);
    mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SnippetDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SnippetDialog::reject);
    mainLayout->addWidget(buttonBox);
    mUi->setupUi(mainWidget);

    mUi->keyWidget->setCheckActionCollections(QList<KActionCollection *>() << actionCollection);
    mOkButton->setEnabled(false);

    connect(mUi->nameEdit, &KLineEdit::textChanged, this, &SnippetDialog::slotTextChanged);
    connect(mUi->groupBox, QOverload<int>::of(&KComboBox::currentIndexChanged), this, &SnippetDialog::slotGroupChanged);

    mUi->snippetText->setMinimumSize(500, 300);

    mUi->groupWidget->setVisible(!inGroupMode);
    mUi->nameEdit->setFocus();
    MessageComposer::ConvertSnippetVariableMenu *variableMenu = new MessageComposer::ConvertSnippetVariableMenu(this, this);
    mUi->pushButtonVariables->setMenu(variableMenu->menu());
    connect(variableMenu, &MessageComposer::ConvertSnippetVariableMenu::insertVariable, this, [this](MessageComposer::ConvertSnippetVariablesUtil::VariableType type) {
        mUi->snippetText->editor()->insertPlainText(MessageComposer::ConvertSnippetVariablesUtil::snippetVariableFromEnum(type));
    });
    readConfig();
}

SnippetDialog::~SnippetDialog()
{
    writeConfig();
    delete mUi;
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
    mUi->nameEdit->setText(name);
}

QString SnippetDialog::name() const
{
    return mUi->nameEdit->text();
}

void SnippetDialog::setText(const QString &text)
{
    mUi->snippetText->setPlainText(text);
}

QString SnippetDialog::text() const
{
    return mUi->snippetText->toPlainText();
}

void SnippetDialog::setKeySequence(const QKeySequence &sequence)
{
    mUi->keyWidget->setKeySequence(sequence);
}

QKeySequence SnippetDialog::keySequence() const
{
    return mUi->keyWidget->keySequence();
}

void SnippetDialog::setKeyword(const QString &keyword)
{
    mUi->keyword->setText(keyword);
}

QString SnippetDialog::keyword() const
{
    return mUi->keyword->text();
}

void SnippetDialog::setGroupModel(QAbstractItemModel *model)
{
    mUi->groupBox->setModel(model);
}

void SnippetDialog::setGroupIndex(const QModelIndex &index)
{
    mUi->groupBox->setCurrentIndex(index.row());
}

QModelIndex SnippetDialog::groupIndex() const
{
    return mUi->groupBox->model()->index(mUi->groupBox->currentIndex(), 0);
}

void SnippetDialog::slotTextChanged()
{
    mOkButton->setEnabled(snippetIsValid());
}

bool SnippetDialog::snippetIsValid() const
{
    if (mUi->nameEdit->text().trimmed().isEmpty()) {
        return false;
    } else {
        if (mUi->groupWidget->isVisible()) {
            return !mUi->groupBox->currentText().trimmed().isEmpty();
        }
    }
    return true;
}
