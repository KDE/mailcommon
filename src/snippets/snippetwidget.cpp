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

#include "snippetwidget.h"
#include "ui_snippetwidget.h"
#include <MessageComposer/ConvertSnippetVariableMenu>
#include <KPIMTextEdit/PlainTextEditor>

#include <KActionCollection>
#include <KComboBox>
#include <KLineEdit>
#include <QAbstractListModel>
#include <QVBoxLayout>
using namespace MailCommon;

class SnippetWidgetPrivate
{
public:
    Ui::SnippetWidget mUi;
    QWidget *wdg = nullptr;
    bool isSelectedGroup = false;
    bool wasChanged = false;
};

SnippetWidget::SnippetWidget(QWidget *parent)
    : QWidget(parent)
    , d(new SnippetWidgetPrivate)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setObjectName(QStringLiteral("mainlayout"));
    layout->setContentsMargins(0, 0, 0, 0);
    d->wdg = new QWidget(this);
    d->mUi.setupUi(d->wdg);
    layout->addWidget(d->wdg);

    MessageComposer::ConvertSnippetVariableMenu *variableMenu = new MessageComposer::ConvertSnippetVariableMenu(this, this);
    d->mUi.pushButtonVariables->setMenu(variableMenu->menu());
    connect(variableMenu, &MessageComposer::ConvertSnippetVariableMenu::insertVariable, this, [this](MessageComposer::ConvertSnippetVariablesUtil::VariableType type) {
        d->mUi.snippetText->editor()->insertPlainText(MessageComposer::ConvertSnippetVariablesUtil::snippetVariableFromEnum(type));
    });

    d->mUi.nameEdit->setTrapReturnKey(true);
    d->mUi.keyword->setTrapReturnKey(true);
    d->mUi.keyword->setClearButtonEnabled(true);
    d->mUi.nameEdit->setClearButtonEnabled(true);
    d->mUi.nameEdit->setFocus();
    d->mUi.snippetText->setMinimumSize(500, 300);

    connect(d->mUi.nameEdit, &KLineEdit::textChanged, this, [this](const QString &str) {
        Q_EMIT textChanged(str);
        d->wasChanged = true;
    });

    connect(d->mUi.groupBox, QOverload<int>::of(&KComboBox::currentIndexChanged), this, [this](int index) {
        Q_EMIT groupChanged(index);
        d->wasChanged = true;
    });
    connect(d->mUi.keyword, &KLineEdit::textChanged, this, [this]() {
        d->wasChanged = true;
    });
    connect(d->mUi.snippetText->editor(), &KPIMTextEdit::PlainTextEditor::textChanged, this, [this]() {
        d->wasChanged = true;
    });
    connect(d->mUi.keyWidget, &KKeySequenceWidget::keySequenceChanged, this, [this]() {
        d->wasChanged = true;
    });
}

SnippetWidget::~SnippetWidget()
{
    delete d;
}

void SnippetWidget::setName(const QString &name)
{
    d->mUi.nameEdit->setText(name);
}

QString SnippetWidget::name() const
{
    return d->mUi.nameEdit->text();
}

void SnippetWidget::setText(const QString &text)
{
    d->mUi.snippetText->setPlainText(text);
}

QString SnippetWidget::text() const
{
    return d->mUi.snippetText->toPlainText();
}

void SnippetWidget::setKeySequence(const QKeySequence &sequence)
{
    d->mUi.keyWidget->setKeySequence(sequence);
}

QKeySequence SnippetWidget::keySequence() const
{
    return d->mUi.keyWidget->keySequence();
}

void SnippetWidget::setKeyword(const QString &keyword)
{
    d->mUi.keyword->setText(keyword);
}

QString SnippetWidget::keyword() const
{
    return d->mUi.keyword->text();
}

void SnippetWidget::setGroupModel(QAbstractItemModel *model)
{
    d->mUi.groupBox->setModel(model);
}

void SnippetWidget::setGroupIndex(const QModelIndex &index)
{
    d->mUi.groupBox->setCurrentIndex(index.row());
}

QModelIndex SnippetWidget::groupIndex() const
{
    return d->mUi.groupBox->model()->index(d->mUi.groupBox->currentIndex(), 0);
}

bool SnippetWidget::snippetIsValid() const
{
    if (d->mUi.nameEdit->text().trimmed().isEmpty()) {
        return false;
    } else {
        if (d->mUi.groupWidget->isVisible()) {
            return !d->mUi.groupBox->currentText().trimmed().isEmpty();
        }
    }
    return true;
}

void SnippetWidget::setCheckActionCollections(const QList<KActionCollection *> &lst)
{
    d->mUi.keyWidget->setCheckActionCollections(lst);
}

void SnippetWidget::setGroupSelected(bool inGroupMode)
{
    d->isSelectedGroup = inGroupMode;
    d->mUi.groupWidget->setVisible(!inGroupMode);
}

bool SnippetWidget::isGroupSelected() const
{
    return d->isSelectedGroup;
}

void SnippetWidget::clear()
{
    d->mUi.nameEdit->clear();
    d->mUi.keyword->clear();
    d->mUi.snippetText->clear();
    d->mUi.keyWidget->setKeySequence({});
}

bool SnippetWidget::wasChanged() const
{
    return d->wasChanged;
}

void SnippetWidget::setWasChanged(bool b)
{
    d->wasChanged = b;
}
