/*

  Copyright (c) 2011-2020 Laurent Montel <montel@kde.org>

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

#include "filteractionmissingfolderdialog.h"
#include "filter/kmfilteraccountlist.h"
#include "folder/folderrequester.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"
#include "tag/addtagdialog.h"

#include <EntityMimeTypeFilterModel>

#include <KIdentityManagement/IdentityCombo>

#include <KLocalizedString>

#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>

FilterActionMissingFolderDialog::FilterActionMissingFolderDialog(
    const Akonadi::Collection::List &list, const QString &filtername, const QString &argStr, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Folder"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *lab = new QLabel(i18n("Folder path was \"%1\".", argStr));
    lab->setObjectName(QStringLiteral("argumentlabel"));
    lab->setWordWrap(true);
    mainLayout->addWidget(lab);
    if (!list.isEmpty()) {
        lab = new QLabel(i18n("The following folders can be used for this filter:"));
        lab->setObjectName(QStringLiteral("label"));
        lab->setWordWrap(true);
        mainLayout->addWidget(lab);
        mListwidget = new QListWidget(this);
        mainLayout->addWidget(mListwidget);
        const int numberOfItems(list.count());
        for (int i = 0; i < numberOfItems; ++i) {
            Akonadi::Collection col = list.at(i);
            QListWidgetItem *item = new QListWidgetItem(MailCommon::Util::fullCollectionPath(col));
            item->setData(FilterActionMissingFolderDialog::IdentifyCollection, col.id());
            mListwidget->addItem(item);
        }
        connect(mListwidget, &QListWidget::currentItemChanged, this, &FilterActionMissingFolderDialog::slotCurrentItemChanged);
        connect(mListwidget, &QListWidget::itemDoubleClicked, this, &FilterActionMissingFolderDialog::slotDoubleItemClicked);
    }

    QLabel *label = new QLabel(this);
    label->setObjectName(QStringLiteral("folderlabel"));
    label->setWordWrap(true);
    if (filtername.isEmpty()) {
        label->setText(i18n("Please select a folder"));
    } else {
        label->setText(i18n("Filter folder is missing. "
                            "Please select a folder to use with filter \"%1\"",
                            filtername));
    }
    mainLayout->addWidget(label);
    mFolderRequester = new MailCommon::FolderRequester(this);
    mFolderRequester->setObjectName(QStringLiteral("folderrequester"));
    connect(mFolderRequester, &MailCommon::FolderRequester::folderChanged, this, &FilterActionMissingFolderDialog::slotFolderChanged);
    mainLayout->addWidget(mFolderRequester);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
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
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingCollectionDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingFolderDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingCollectionDialog");
    group.writeEntry("Size", size());
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

    const Akonadi::Collection::Id id
        = item->data(FilterActionMissingFolderDialog::IdentifyCollection).toLongLong();

    mFolderRequester->setCollection(Akonadi::Collection(id));
    accept();
}

void FilterActionMissingFolderDialog::slotCurrentItemChanged()
{
    QListWidgetItem *currentItem = mListwidget->currentItem();
    if (currentItem) {
        const Akonadi::Collection::Id id
            = currentItem->data(FilterActionMissingFolderDialog::IdentifyCollection).toLongLong();
        mFolderRequester->setCollection(Akonadi::Collection(id));
    }
}

Akonadi::Collection FilterActionMissingFolderDialog::selectedCollection() const
{
    return mFolderRequester->collection();
}

void FilterActionMissingFolderDialog::getPotentialFolders(const QAbstractItemModel *model, const QModelIndex &parentIndex, const QString &lastElement, Akonadi::Collection::List &list)
{
    const int rowCount = model->rowCount(parentIndex);
    for (int row = 0; row < rowCount; ++row) {
        const QModelIndex index = model->index(row, 0, parentIndex);
        if (model->rowCount(index) > 0) {
            getPotentialFolders(model, index, lastElement, list);
        }
        if (model->data(index).toString() == lastElement) {
            list << model->data(
                index, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        }
    }
}

Akonadi::Collection::List FilterActionMissingFolderDialog::potentialCorrectFolders(
    const QString &path, bool &exactPath)
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

        FilterActionMissingFolderDialog::getPotentialFolders(
            KernelIf->collectionModel(), QModelIndex(), lastElement, lst);

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
