/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "snippetwidget.h"
#include "ui_snippetwidget.h"
#include <KPIMTextEdit/PlainTextEditor>
#include <MessageComposer/ConvertSnippetVariableMenu>

#include <KActionCollection>
#include <KLineEdit>
#include <KLocalizedString>
#include <QComboBox>
#include <QVBoxLayout>
using namespace MailCommon;

class Q_DECL_HIDDEN SnippetWidgetPrivate
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
    auto layout = new QVBoxLayout(this);
    layout->setObjectName(QStringLiteral("mainlayout"));
    layout->setContentsMargins(0, 0, 0, 0);
    d->wdg = new QWidget(this);
    d->mUi.setupUi(d->wdg);
    layout->addWidget(d->wdg);

    auto variableMenu = new MessageComposer::ConvertSnippetVariableMenu(false, this, this);
    d->mUi.pushButtonVariables->setMenu(variableMenu->menu());
    connect(variableMenu,
            &MessageComposer::ConvertSnippetVariableMenu::insertVariable,
            this,
            [this](MessageComposer::ConvertSnippetVariablesUtil::VariableType type) {
                d->mUi.snippetText->editor()->insertPlainText(MessageComposer::ConvertSnippetVariablesUtil::snippetVariableFromEnum(type) + QLatin1Char(' '));
            });

    d->mUi.nameEdit->setTrapReturnKey(true);
    d->mUi.keyword->setTrapReturnKey(true);
    d->mUi.keyword->setClearButtonEnabled(true);
    d->mUi.nameEdit->setClearButtonEnabled(true);
    d->mUi.nameEdit->setFocus();
    d->mUi.snippetText->setMinimumSize(500, 300);

    d->mUi.keyword->setWhatsThis(
        i18n("Enter a keyword here to enable fast insertion of this snippet while writing "
             "an email. For instance if you choose \"greeting\" as the keyword, you can then "
             "type \\greeting in your email and then press the tab key, and it will be "
             "replaced with the contents of this snippet."));

    connect(d->mUi.nameEdit, &KLineEdit::textChanged, this, [this](const QString &str) {
        Q_EMIT textChanged(str);
        d->wasChanged = true;
    });
    connect(d->mUi.groupBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
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
    connect(d->mUi.cc, &Akonadi::EmailAddressRequester::textChanged, this, [this]() {
        d->wasChanged = true;
    });
    connect(d->mUi.to, &Akonadi::EmailAddressRequester::textChanged, this, [this]() {
        d->wasChanged = true;
    });
    connect(d->mUi.bcc, &Akonadi::EmailAddressRequester::textChanged, this, [this]() {
        d->wasChanged = true;
    });
    connect(d->mUi.subject, &QLineEdit::textChanged, this, [this]() {
        d->wasChanged = true;
    });
    connect(d->mUi.attachment, &MailCommon::SnippetAttachmentWidget::wasChanged, this, [this]() {
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

void SnippetWidget::setTo(const QString &keyword)
{
    d->mUi.to->setText(keyword);
}

QString SnippetWidget::to() const
{
    return d->mUi.to->text();
}

void SnippetWidget::setCc(const QString &keyword)
{
    d->mUi.cc->setText(keyword);
}

QString SnippetWidget::cc() const
{
    return d->mUi.cc->text();
}

void SnippetWidget::setBcc(const QString &keyword)
{
    d->mUi.bcc->setText(keyword);
}

QString SnippetWidget::bcc() const
{
    return d->mUi.bcc->text();
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
    d->mUi.subject->clear();
    d->mUi.cc->clear();
    d->mUi.to->clear();
    d->mUi.bcc->clear();
    d->mUi.attachment->clear();
}

bool SnippetWidget::wasChanged() const
{
    return d->wasChanged;
}

void SnippetWidget::setWasChanged(bool b)
{
    d->wasChanged = b;
}

QString SnippetWidget::subject() const
{
    return d->mUi.subject->text();
}

void SnippetWidget::setAttachment(const QString &keyword)
{
    d->mUi.attachment->setText(keyword);
}

QString SnippetWidget::attachment() const
{
    return d->mUi.attachment->text();
}

void SnippetWidget::setSubject(const QString &text)
{
    d->mUi.subject->setText(text);
}
