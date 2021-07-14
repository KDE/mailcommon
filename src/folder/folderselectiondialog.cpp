/*

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "folderselectiondialog.h"

#include "foldersettings.h"
#include "foldertreeview.h"
#include "foldertreewidget.h"
#include "foldertreewidgetproxymodel.h"
#include "kernel/mailkernel.h"

#include <CollectionCreateJob>
#include <EntityMimeTypeFilterModel>
#include <EntityTreeModel>

#include <KLocalizedString>
#include <KMessageBox>
#include <QInputDialog>
#include <QMenu>

#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QShowEvent>
#include <QVBoxLayout>

namespace MailCommon
{
class Q_DECL_HIDDEN FolderSelectionDialog::FolderSelectionDialogPrivate
{
public:
    FolderTreeWidget *folderTreeWidget = nullptr;
    QPushButton *mUser1Button = nullptr;
    QPushButton *mOkButton = nullptr;
    bool mNotAllowToCreateNewFolder = false;
    bool mUseGlobalSettings = true;
};

FolderSelectionDialog::FolderSelectionDialog(QWidget *parent, SelectionFolderOptions options)
    : QDialog(parent)
    , d(new FolderSelectionDialogPrivate())
{
    setObjectName(QStringLiteral("folder dialog"));

    d->mNotAllowToCreateNewFolder = (options & FolderSelectionDialog::NotAllowToCreateNewFolder);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    auto mainLayout = new QVBoxLayout(this);
    d->mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    d->mOkButton->setDefault(true);
    d->mOkButton->setAutoDefault(true);
    d->mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FolderSelectionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FolderSelectionDialog::reject);

    if (!d->mNotAllowToCreateNewFolder) {
        d->mUser1Button = new QPushButton;
        d->mUser1Button->setDefault(false);
        d->mUser1Button->setAutoDefault(false);
        buttonBox->addButton(d->mUser1Button, QDialogButtonBox::ActionRole);
        KGuiItem::assign(d->mUser1Button,
                         KGuiItem(i18n("&New Subfolder..."), QStringLiteral("folder-new"), i18n("Create a new subfolder under the currently selected folder")));
    }
    FolderTreeWidget::TreeViewOptions opt = FolderTreeWidget::None;
    if (options & FolderSelectionDialog::ShowUnreadCount) {
        opt |= FolderTreeWidget::ShowUnreadCount;
    }
    opt |= FolderTreeWidget::UseDistinctSelectionModel;

    FolderTreeWidgetProxyModel::FolderTreeWidgetProxyModelOptions optReadableProxy = FolderTreeWidgetProxyModel::None;

    if (options & FolderSelectionDialog::HideVirtualFolder) {
        optReadableProxy |= FolderTreeWidgetProxyModel::HideVirtualFolder;
    }

    optReadableProxy |= FolderTreeWidgetProxyModel::HideSpecificFolder;

    if (options & FolderSelectionDialog::HideOutboxFolder) {
        optReadableProxy |= FolderTreeWidgetProxyModel::HideOutboxFolder;
    }

    d->folderTreeWidget = new FolderTreeWidget(this, nullptr, opt, optReadableProxy);
    d->folderTreeWidget->readConfig();
    d->folderTreeWidget->disableContextMenuAndExtraColumn();
    d->folderTreeWidget->folderTreeWidgetProxyModel()->setEnabledCheck((options & EnableCheck));
    // Necessary otherwise we overwrite tooltip config for all application
    d->folderTreeWidget->folderTreeView()->disableSaveConfig();
    d->folderTreeWidget->folderTreeView()->setTooltipsPolicy(FolderTreeWidget::DisplayNever);
#ifndef QT_NO_DRAGANDDROP
    d->folderTreeWidget->folderTreeView()->setDragDropMode(QAbstractItemView::NoDragDrop);
#endif
    mainLayout->addWidget(d->folderTreeWidget);
    mainLayout->addWidget(buttonBox);

    d->mOkButton->setEnabled(false);
    if (!d->mNotAllowToCreateNewFolder) {
        d->mUser1Button->setEnabled(false);
        connect(d->mUser1Button, &QPushButton::clicked, this, &FolderSelectionDialog::slotAddChildFolder);
        d->folderTreeWidget->folderTreeView()->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(d->folderTreeWidget->folderTreeView(),
                &QWidget::customContextMenuRequested,
                this,
                &FolderSelectionDialog::slotFolderTreeWidgetContextMenuRequested);
    }

    connect(d->folderTreeWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FolderSelectionDialog::slotSelectionChanged);
    connect(d->folderTreeWidget->folderTreeWidgetProxyModel(), &QAbstractItemModel::rowsInserted, this, &FolderSelectionDialog::rowsInserted);

    connect(d->folderTreeWidget->folderTreeView(),
            QOverload<const QModelIndex &>::of(&QAbstractItemView::doubleClicked),
            this,
            &FolderSelectionDialog::slotDoubleClick);

    d->mUseGlobalSettings = !(options & NotUseGlobalSettings);
    readConfig();
}

FolderSelectionDialog::~FolderSelectionDialog()
{
    writeConfig();
    delete d;
}

void FolderSelectionDialog::slotFolderTreeWidgetContextMenuRequested(const QPoint &pos)
{
    if (d->mUser1Button && d->mUser1Button->isEnabled() && d->folderTreeWidget->folderTreeView()->indexAt(pos).isValid()) {
        QMenu menu(this);
        menu.addAction(i18n("&New Subfolder..."), this, &FolderSelectionDialog::slotAddChildFolder);
        menu.exec(QCursor::pos());
    }
}

void FolderSelectionDialog::slotDoubleClick(const QModelIndex &index)
{
    Q_UNUSED(index)
    const bool hasSelectedCollection = (!d->folderTreeWidget->selectionModel()->selectedIndexes().isEmpty());
    if (hasSelectedCollection) {
        accept();
    }
}

void FolderSelectionDialog::focusTreeView()
{
    d->folderTreeWidget->folderTreeView()->expandAll();
    d->folderTreeWidget->folderTreeView()->setFocus();
}

void FolderSelectionDialog::showEvent(QShowEvent *event)
{
    if (!event->spontaneous()) {
        focusTreeView();
        FolderTreeView *view = d->folderTreeWidget->folderTreeView();
        view->scrollTo(view->currentIndex());
    }
    QDialog::showEvent(event);
}

void FolderSelectionDialog::rowsInserted(const QModelIndex &, int, int)
{
    d->folderTreeWidget->folderTreeView()->expandAll();
}

bool FolderSelectionDialog::canCreateCollection(Akonadi::Collection &parentCol)
{
    parentCol = selectedCollection();
    if (!parentCol.isValid()) {
        return false;
    }

    if ((parentCol.rights() & Akonadi::Collection::CanCreateCollection) && parentCol.contentMimeTypes().contains(Akonadi::Collection::mimeType())) {
        return true;
    }
    return false;
}

void FolderSelectionDialog::slotAddChildFolder()
{
    Akonadi::Collection parentCol;
    if (canCreateCollection(parentCol)) {
        bool ok = false;
        const QString name = QInputDialog::getText(this, i18nc("@title:window", "New Folder"), i18nc("@label:textbox, name of a thing", "Name"), {}, {}, &ok);

        if (name.isEmpty() || !ok) {
            return;
        }

        Akonadi::Collection col;
        col.setName(name);
        col.parentCollection().setId(parentCol.id());
        auto job = new Akonadi::CollectionCreateJob(col);
        connect(job, &Akonadi::CollectionCreateJob::result, this, &FolderSelectionDialog::collectionCreationResult);
    }
}

void FolderSelectionDialog::collectionCreationResult(KJob *job)
{
    if (job->error()) {
        KMessageBox::error(this, i18n("Could not create folder: %1", job->errorString()), i18n("Folder creation failed"));
    }
}

void FolderSelectionDialog::slotSelectionChanged()
{
    const bool enablebuttons = (!d->folderTreeWidget->selectionModel()->selectedIndexes().isEmpty());
    d->mOkButton->setEnabled(enablebuttons);

    if (!d->mNotAllowToCreateNewFolder) {
        Akonadi::Collection parent;
        d->mUser1Button->setEnabled(canCreateCollection(parent));
        if (parent.isValid()) {
            const QSharedPointer<FolderSettings> fd(FolderSettings::forCollection(parent, false));
            d->mOkButton->setEnabled(fd->canCreateMessages());
        }
    }
}

void FolderSelectionDialog::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    d->folderTreeWidget->setSelectionMode(mode);
}

QAbstractItemView::SelectionMode FolderSelectionDialog::selectionMode() const
{
    return d->folderTreeWidget->selectionMode();
}

Akonadi::Collection FolderSelectionDialog::selectedCollection() const
{
    return d->folderTreeWidget->selectedCollection();
}

void FolderSelectionDialog::setSelectedCollection(const Akonadi::Collection &collection)
{
    d->folderTreeWidget->selectCollectionFolder(collection);
}

Akonadi::Collection::List FolderSelectionDialog::selectedCollections() const
{
    return d->folderTreeWidget->selectedCollections();
}

static const char myConfigGroupName[] = "FolderSelectionDialog";

void FolderSelectionDialog::readConfig()
{
    KConfigGroup group(KernelIf->config(), myConfigGroupName);

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
    if (d->mUseGlobalSettings) {
        const Akonadi::Collection::Id id = SettingsIf->lastSelectedFolder();
        if (id > -1) {
            const Akonadi::Collection col = Kernel::self()->collectionFromId(id);
            d->folderTreeWidget->selectCollectionFolder(col);
        }
    }
}

void FolderSelectionDialog::writeConfig()
{
    KConfigGroup group(KernelIf->config(), myConfigGroupName);
    group.writeEntry("Size", size());

    if (d->mUseGlobalSettings) {
        Akonadi::Collection col = selectedCollection();
        if (col.isValid()) {
            SettingsIf->setLastSelectedFolder(col.id());
        }
    }
}

void FolderSelectionDialog::hideEvent(QHideEvent *)
{
    d->folderTreeWidget->clearFilter();
}
}
