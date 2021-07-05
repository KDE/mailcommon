/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "addtagdialog.h"
#include "mailcommon_debug.h"
#include "tag/tagwidget.h"

#include <KJob>
#include <KLocalizedString>
#include <KMessageBox>
#include <QLineEdit>

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace MailCommon;
class MailCommon::AddTagDialogPrivate
{
public:
    AddTagDialogPrivate()
    {
    }

    QString mLabel;
    QString mGid;
    MailCommon::TagWidget *mTagWidget = nullptr;
    QVector<MailCommon::Tag::Ptr> mTags;
    Akonadi::Tag mTag;
    QPushButton *mOkButton = nullptr;
};

AddTagDialog::AddTagDialog(const QList<KActionCollection *> &actions, QWidget *parent)
    : QDialog(parent)
    , d(new MailCommon::AddTagDialogPrivate)
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Add Tag"));
    auto mainLayout = new QVBoxLayout(this);

    d->mTagWidget = new MailCommon::TagWidget(actions, this);
    mainLayout->addWidget(d->mTagWidget);

    connect(d->mTagWidget->tagNameLineEdit(), &QLineEdit::textChanged, this, &AddTagDialog::slotTagNameChanged);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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

void AddTagDialog::setTags(const QVector<MailCommon::Tag::Ptr> &tags)
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

    for (const MailCommon::Tag::Ptr &tag : std::as_const(d->mTags)) {
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
    auto createJob = new Akonadi::TagCreateJob(akonadiTag, this);
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
    auto createJob = static_cast<Akonadi::TagCreateJob *>(job);
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
