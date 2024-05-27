/*

  SPDX-FileCopyrightText: 2011-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filteractionmissingfolderdialog.h"
#include "folder/folderrequester.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"

#include <Akonadi/EntityMimeTypeFilterModel>

#include <KLocalizedString>

#include <KConfigGroup>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
static const char myFilterActionMissingCollectionDialogConfigGroupName[] = "FilterActionMissingCollectionDialog";
}

FilterActionMissingFolderDialog::FilterActionMissingFolderDialog(const Akonadi::Collection::List &list,
                                                                 const QString &filtername,
                                                                 const QString &argStr,
                                                                 QWidget *parent)
    : QDialog(parent)
    , mFolderRequester(new MailCommon::FolderRequester(this))
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Folder"));
    auto mainLayout = new QVBoxLayout(this);

    auto lab = new QLabel(i18nc("@label:textbox", "Folder path was \"%1\".", argStr));
    lab->setObjectName(QLatin1StringView("argumentlabel"));
    lab->setWordWrap(true);
    mainLayout->addWidget(lab);
    if (!list.isEmpty()) {
        lab = new QLabel(i18nc("@label:textbox", "The following folders can be used for this filter:"));
        lab->setObjectName(QLatin1StringView("label"));
        lab->setWordWrap(true);
        mainLayout->addWidget(lab);
        mListwidget = new QListWidget(this);
        mainLayout->addWidget(mListwidget);
        const int numberOfItems(list.count());
        for (int i = 0; i < numberOfItems; ++i) {
            const Akonadi::Collection col = list.at(i);
            auto item = new QListWidgetItem(MailCommon::Util::fullCollectionPath(col));
            item->setData(FilterActionMissingFolderDialog::IdentifyCollection, col.id());
            mListwidget->addItem(item);
        }
        connect(mListwidget, &QListWidget::currentItemChanged, this, &FilterActionMissingFolderDialog::slotCurrentItemChanged);
        connect(mListwidget, &QListWidget::itemDoubleClicked, this, &FilterActionMissingFolderDialog::slotDoubleItemClicked);
    }

    auto label = new QLabel(this);
    label->setObjectName(QLatin1StringView("folderlabel"));
    label->setWordWrap(true);
    if (filtername.isEmpty()) {
        label->setText(i18n("Please select a folder:"));
    } else {
        label->setText(
            i18n("Filter folder is missing. "
                 "Please select a folder to use with filter \"%1\":",
                 filtername));
    }
    mainLayout->addWidget(label);
    mFolderRequester->setObjectName(QLatin1StringView("folderrequester"));
    connect(mFolderRequester, &MailCommon::FolderRequester::folderChanged, this, &FilterActionMissingFolderDialog::slotFolderChanged);
    mainLayout->addWidget(mFolderRequester);
    mainLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QLatin1StringView("buttonbox"));
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setDefault(true);
    mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    mOkButton->setEnabled(false);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingFolderDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingFolderDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

FilterActionMissingFolderDialog::~FilterActionMissingFolderDialog()
{
    writeConfig();
}

void FilterActionMissingFolderDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myFilterActionMissingCollectionDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void FilterActionMissingFolderDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myFilterActionMissingCollectionDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

void FilterActionMissingFolderDialog::slotFolderChanged(const Akonadi::Collection &col)
{
    mOkButton->setEnabled(col.isValid());
}

void FilterActionMissingFolderDialog::slotDoubleItemClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }

    const Akonadi::Collection::Id id = item->data(FilterActionMissingFolderDialog::IdentifyCollection).toLongLong();

    mFolderRequester->setCollection(Akonadi::Collection(id));
    accept();
}

void FilterActionMissingFolderDialog::slotCurrentItemChanged()
{
    QListWidgetItem *currentItem = mListwidget->currentItem();
    if (currentItem) {
        const Akonadi::Collection::Id id = currentItem->data(FilterActionMissingFolderDialog::IdentifyCollection).toLongLong();
        mFolderRequester->setCollection(Akonadi::Collection(id));
    }
}

Akonadi::Collection FilterActionMissingFolderDialog::selectedCollection() const
{
    return mFolderRequester->collection();
}

void FilterActionMissingFolderDialog::getPotentialFolders(const QAbstractItemModel *model,
                                                          const QModelIndex &parentIndex,
                                                          const QString &lastElement,
                                                          Akonadi::Collection::List &list)
{
    const int rowCount = model->rowCount(parentIndex);
    for (int row = 0; row < rowCount; ++row) {
        const QModelIndex index = model->index(row, 0, parentIndex);
        if (model->rowCount(index) > 0) {
            getPotentialFolders(model, index, lastElement, list);
        }
        if (model->data(index).toString() == lastElement) {
            list << model->data(index, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        }
    }
}

Akonadi::Collection::List FilterActionMissingFolderDialog::potentialCorrectFolders(const QString &path, bool &exactPath)
{
    Akonadi::Collection::List lst;
    const QString realPath = MailCommon::Util::realFolderPath(path);
    if (realPath.isEmpty()) {
        return lst;
    }

    if (KernelIf->collectionModel()) {
        const int lastSlash = realPath.lastIndexOf(QLatin1Char('/'));
        QString lastElement;
        if (lastSlash == -1) {
            lastElement = realPath;
        } else {
            lastElement = realPath.right(realPath.length() - lastSlash - 1);
        }

        FilterActionMissingFolderDialog::getPotentialFolders(KernelIf->collectionModel(), QModelIndex(), lastElement, lst);

        const int numberOfItems(lst.count());
        for (int i = 0; i < numberOfItems; ++i) {
            if (MailCommon::Util::fullCollectionPath(lst.at(i)) == realPath) {
                exactPath = true;
                return Akonadi::Collection::List() << lst.at(i);
            }
        }
    }
    return lst;
}

#include "moc_filteractionmissingfolderdialog.cpp"
