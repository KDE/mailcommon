/*
  Copyright (c) 2012-2019 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "addtagdialog.h"
#include "tag/tagwidget.h"
#include "mailcommon_debug.h"

#include <KLocalizedString>
#include <KLineEdit>
#include <KMessageBox>
#include <KJob>

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>

using namespace MailCommon;
class MailCommon::AddTagDialogPrivate
{
public:
    AddTagDialogPrivate()
        : mTagWidget(nullptr)
        , mOkButton(nullptr)
    {
    }

    QString mLabel;
    QString mGid;
    MailCommon::TagWidget *mTagWidget = nullptr;
    QList<MailCommon::Tag::Ptr> mTags;
    Akonadi::Tag mTag;
    QPushButton *mOkButton = nullptr;
};

AddTagDialog::AddTagDialog(const QList<KActionCollection *> &actions, QWidget *parent)
    : QDialog(parent)
    , d(new MailCommon::AddTagDialogPrivate)
{
    setModal(true);
    setWindowTitle(i18n("Add Tag"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    d->mTagWidget = new MailCommon::TagWidget(actions, this);
    mainLayout->addWidget(d->mTagWidget);

    connect(d->mTagWidget->tagNameLineEdit(), &KLineEdit::textChanged, this, &AddTagDialog::slotTagNameChanged);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    d->mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    d->mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddTagDialog::slotSave);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AddTagDialog::reject);
    d->mOkButton->setDefault(true);
    d->mOkButton->setEnabled(false);

    mainLayout->addWidget(buttonBox);
}

AddTagDialog::~AddTagDialog()
{
    delete d;
}

void AddTagDialog::setTags(const QList<MailCommon::Tag::Ptr> &tags)
{
    d->mTags = tags;
}

void AddTagDialog::slotTagNameChanged(const QString &text)
{
    d->mOkButton->setEnabled(!text.trimmed().isEmpty());
}

void AddTagDialog::slotSave()
{
    const QString name(d->mTagWidget->tagNameLineEdit()->text());

    for (const MailCommon::Tag::Ptr &tag : qAsConst(d->mTags)) {
        if (tag->name() == name) {
            KMessageBox::error(this, i18n("Tag %1 already exists", name));
            d->mTagWidget->tagNameLineEdit()->setFocus();
            d->mTagWidget->tagNameLineEdit()->selectAll();
            return;
        }
    }

    MailCommon::Tag::Ptr tag(Tag::createDefaultTag(name));
    d->mTagWidget->recordTagSettings(tag);
    const Akonadi::Tag akonadiTag = tag->saveToAkonadi();
    Akonadi::TagCreateJob *createJob = new Akonadi::TagCreateJob(akonadiTag, this);
    connect(createJob, &Akonadi::TagCreateJob::result, this, &AddTagDialog::onTagCreated);

    d->mLabel = name;
}

void AddTagDialog::onTagCreated(KJob *job)
{
    if (job->error()) {
        qCWarning(MAILCOMMON_LOG) << "Failed to create tag: " << job->errorString();
        reject();
        return;
    }
    Akonadi::TagCreateJob *createJob = static_cast<Akonadi::TagCreateJob *>(job);
    d->mTag = createJob->tag();
    accept();
}

QString AddTagDialog::label() const
{
    return d->mLabel;
}

Akonadi::Tag AddTagDialog::tag() const
{
    return d->mTag;
}
