/*
 * SPDX-FileCopyrightText: 2004 Carsten Burghardt <burghardt@kde.org>
 * SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only
 */

#include "folderrequester.h"
#include "folderselectiondialog.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"
#include <CollectionFetchJob>
#include <Libkdepim/LineEditCatchReturnKey>

#include <KLocalizedString>
#include <QIcon>
#include <QLineEdit>
#include <QPointer>

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QToolButton>

namespace MailCommon
{
class FolderRequesterPrivate
{
public:
    Akonadi::Collection mCollection;
    QLineEdit *mEdit = nullptr;
    QString mSelectFolderTitleDialog;
    bool mMustBeReadWrite = false;
    bool mShowOutbox = false;
    bool mNotCreateNewFolder = false;
};

FolderRequester::FolderRequester(QWidget *parent)
    : QWidget(parent)
    , d(new MailCommon::FolderRequesterPrivate)
{
    auto hlay = new QHBoxLayout(this);
    hlay->setContentsMargins(0, 0, 0, 0);

    d->mEdit = new QLineEdit(this);
    d->mEdit->setPlaceholderText(i18n("Select Folder"));
    new KPIM::LineEditCatchReturnKey(d->mEdit, this);
    d->mEdit->setReadOnly(true);
    hlay->addWidget(d->mEdit);

    auto button = new QToolButton(this);
    button->setIcon(QIcon::fromTheme(QStringLiteral("folder")));
    hlay->addWidget(button);
    connect(button, &QToolButton::clicked, this, &FolderRequester::slotOpenDialog);

    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    setFocusPolicy(Qt::StrongFocus);
}

//-----------------------------------------------------------------------------
void FolderRequester::slotOpenDialog()
{
    FolderSelectionDialog::SelectionFolderOptions options = FolderSelectionDialog::EnableCheck;
    options |= FolderSelectionDialog::HideVirtualFolder;
    options |= FolderSelectionDialog::NotUseGlobalSettings;
    if (d->mNotCreateNewFolder) {
        options |= FolderSelectionDialog::NotAllowToCreateNewFolder;
    }
    if (!d->mShowOutbox) {
        options |= FolderSelectionDialog::HideOutboxFolder;
    }

    QPointer<FolderSelectionDialog> dlg(new FolderSelectionDialog(this, options));
    dlg->setWindowTitle(d->mSelectFolderTitleDialog.isEmpty() ? i18nc("@title:window", "Select Folder") : d->mSelectFolderTitleDialog);
    dlg->setModal(false);
    dlg->setSelectedCollection(d->mCollection);

    if (dlg->exec() && dlg) {
        setCollection(dlg->selectedCollection(), false);
    }
    delete dlg;
}

//-----------------------------------------------------------------------------
FolderRequester::~FolderRequester()
{
    delete d;
}

Akonadi::Collection FolderRequester::collection() const
{
    return d->mCollection;
}

void FolderRequester::setCollectionFullPath(const Akonadi::Collection &col)
{
    if (KernelIf->collectionModel()) {
        d->mEdit->setText(Util::fullCollectionPath(col));
    } else {
        d->mEdit->clear();
    }
}

//-----------------------------------------------------------------------------
void FolderRequester::setCollection(const Akonadi::Collection &collection, bool fetchCollection)
{
    d->mCollection = collection;
    if (d->mCollection.isValid()) {
        if (fetchCollection) {
            auto *job = new Akonadi::CollectionFetchJob(d->mCollection, Akonadi::CollectionFetchJob::Base, this);

            connect(job, &Akonadi::CollectionFetchJob::result, this, &FolderRequester::slotCollectionsReceived);
        } else {
            setCollectionFullPath(d->mCollection);
        }
    } else if (!d->mMustBeReadWrite) { // the Local Folders root node was selected
        d->mEdit->setText(i18n("Local Folders"));
    }

    Q_EMIT folderChanged(d->mCollection);
}

void FolderRequester::slotCollectionsReceived(KJob *job)
{
    if (job->error()) {
        d->mCollection = Akonadi::Collection();
        d->mEdit->setText(i18n("Please select a folder"));
        Q_EMIT invalidFolder();
        return;
    }

    const Akonadi::CollectionFetchJob *fetchJob = qobject_cast<Akonadi::CollectionFetchJob *>(job);
    const Akonadi::Collection::List collections = fetchJob->collections();

    if (!collections.isEmpty()) {
        const Akonadi::Collection collection = collections.first();
        // in case this is still the collection we are interested in, update
        if (collection.id() == d->mCollection.id()) {
            d->mCollection = collection;
            setCollectionFullPath(collection);
        }
    } else {
        // the requested collection doesn't exists anymore
        d->mCollection = Akonadi::Collection();
        d->mEdit->setText(i18n("Please select a folder"));
        Q_EMIT invalidFolder();
    }
}

bool FolderRequester::hasCollection() const
{
    return d->mCollection.isValid();
}

//-----------------------------------------------------------------------------
void FolderRequester::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Space) {
        slotOpenDialog();
    } else {
        e->ignore();
    }
}

void FolderRequester::setMustBeReadWrite(bool readwrite)
{
    d->mMustBeReadWrite = readwrite;
}

void FolderRequester::setShowOutbox(bool show)
{
    d->mShowOutbox = show;
}

void FolderRequester::setNotAllowToCreateNewFolder(bool notCreateNewFolder)
{
    d->mNotCreateNewFolder = notCreateNewFolder;
}

void FolderRequester::setSelectFolderTitleDialog(const QString &title)
{
    d->mSelectFolderTitleDialog = title;
}
} // namespace MailCommon
