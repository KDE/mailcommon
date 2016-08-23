/*

  Copyright (c) 2011-2016 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "filteractionmissingargumentdialog.h"
#include "filter/kmfilteraccountlist.h"
#include "folder/folderrequester.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"
#include "tag/addtagdialog.h"

#include <EntityMimeTypeFilterModel>

#include <MailTransport/TransportComboBox>
#include <MailTransport/Transport>
#include <MailTransport/TransportManager>

#include <KIdentityManagement/IdentityCombo>

#include <KLocalizedString>
#include <KUrlRequester>

#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>

FilterActionMissingCollectionDialog::FilterActionMissingCollectionDialog(
    const Akonadi::Collection::List &list, const QString &filtername,
    const QString &argStr, QWidget *parent)
    : QDialog(parent),
      mListwidget(Q_NULLPTR)
{
    setModal(true);
    setWindowTitle(i18n("Select Folder"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QWidget *mainWidget = new QWidget(this);
    mainLayout->addWidget(mainWidget);
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setDefault(true);
    mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingCollectionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingCollectionDialog::reject);
    mainLayout->addWidget(buttonBox);
    mOkButton->setDefault(true);
    QVBoxLayout *lay = new QVBoxLayout(mainWidget);
    QLabel *lab = new QLabel(i18n("Folder path was \"%1\".", argStr));
    lab->setWordWrap(true);
    lay->addWidget(lab);
    if (!list.isEmpty()) {
        lab = new QLabel(i18n("The following folders can be used for this filter:"));
        lab->setWordWrap(true);
        lay->addWidget(lab);
        mListwidget = new QListWidget(this);
        lay->addWidget(mListwidget);
        const int numberOfItems(list.count());
        for (int i = 0; i < numberOfItems; ++i) {
            Akonadi::Collection col = list.at(i);
            QListWidgetItem *item = new QListWidgetItem(MailCommon::Util::fullCollectionPath(col));
            item->setData(FilterActionMissingCollectionDialog::IdentifyCollection, col.id());
            mListwidget->addItem(item);
        }
        connect(mListwidget, &QListWidget::currentItemChanged, this, &FilterActionMissingCollectionDialog::slotCurrentItemChanged);
        connect(mListwidget, &QListWidget::itemDoubleClicked, this, &FilterActionMissingCollectionDialog::slotDoubleItemClicked);

    }

    QLabel *label = new QLabel(this);
    label->setWordWrap(true);
    if (filtername.isEmpty()) {
        label->setText(i18n("Please select a folder"));
    } else
        label->setText(i18n("Filter folder is missing. "
                            "Please select a folder to use with filter \"%1\"",
                            filtername));
    lay->addWidget(label);
    mFolderRequester = new MailCommon::FolderRequester(this);
    connect(mFolderRequester, &MailCommon::FolderRequester::folderChanged, this, &FilterActionMissingCollectionDialog::slotFolderChanged);
    lay->addWidget(mFolderRequester);
    mOkButton->setEnabled(false);
    readConfig();
}

FilterActionMissingCollectionDialog::~FilterActionMissingCollectionDialog()
{
    writeConfig();
}

void FilterActionMissingCollectionDialog::readConfig()
{
    KConfigGroup group(KernelIf->config(), "FilterActionMissingCollectionDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingCollectionDialog::writeConfig()
{
    KConfigGroup group(KernelIf->config(), "FilterActionMissingCollectionDialog");
    group.writeEntry("Size", size());
}

void FilterActionMissingCollectionDialog::slotFolderChanged(const Akonadi::Collection &col)
{
    mOkButton->setEnabled(col.isValid());
}

void FilterActionMissingCollectionDialog::slotDoubleItemClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }

    const Akonadi::Collection::Id id =
        item->data(FilterActionMissingCollectionDialog::IdentifyCollection).toLongLong();

    mFolderRequester->setCollection(Akonadi::Collection(id));
    accept();
}

void FilterActionMissingCollectionDialog::slotCurrentItemChanged()
{
    QListWidgetItem *currentItem = mListwidget->currentItem();
    if (currentItem) {
        const Akonadi::Collection::Id id =
            currentItem->data(FilterActionMissingCollectionDialog::IdentifyCollection).toLongLong();
        mFolderRequester->setCollection(Akonadi::Collection(id));
    }
}

Akonadi::Collection FilterActionMissingCollectionDialog::selectedCollection() const
{
    return mFolderRequester->collection();
}

void FilterActionMissingCollectionDialog::getPotentialFolders(const QAbstractItemModel *model,
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
            list << model->data(
                     index, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        }
    }
}

Akonadi::Collection::List FilterActionMissingCollectionDialog::potentialCorrectFolders(
    const QString &path, bool &exactPath)
{
    Akonadi::Collection::List lst;
    const QString realPath = MailCommon::Util::realFolderPath(path);
    if (realPath.isEmpty()) {
        return lst;
    }

    const int lastSlash = realPath.lastIndexOf(QLatin1Char('/'));
    QString lastElement;
    if (lastSlash == -1) {
        lastElement = realPath;
    } else {
        lastElement = realPath.right(realPath.length() - lastSlash - 1);
    }

    if (KernelIf->collectionModel()) {
        FilterActionMissingCollectionDialog::getPotentialFolders(
            KernelIf->collectionModel(), QModelIndex(), lastElement, lst);

        const int numberOfItems(lst.count());
        for (int i = 0; i < numberOfItems; ++i) {
            if (MailCommon::Util::fullCollectionPath(lst.at(i)) == realPath) {
                exactPath = true;
                return  Akonadi::Collection::List() << lst.at(i);
            }
        }
    }
    return lst;
}

