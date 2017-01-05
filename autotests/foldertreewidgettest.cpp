/*
  Copyright (c) 2016 David Faure <faure@kde.org>

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

#undef QT_NO_CAST_FROM_ASCII

#include <qtest_akonadi.h>

#include <AkonadiCore/Item>
#include <AkonadiCore/Collection>
#include <AkonadiCore/ItemFetchScope>

#include <AkonadiCore/AgentType>
#include <AkonadiCore/AgentInstanceCreateJob>
#include <AkonadiCore/AgentManager>
#include <AkonadiCore/SearchCreateJob>
#include <AkonadiCore/SearchQuery>
#include <AkonadiCore/CollectionDeleteJob>
#include <AkonadiCore/CollectionCreateJob>
#include <AkonadiCore/CollectionFetchJob>
#include <AkonadiCore/CollectionMoveJob>
#include <AkonadiCore/EntityOrderProxyModel>

#include <KMime/Message>

#include "folder/entitycollectionorderproxymodel.h"
#include <MailCommon/FolderTreeWidget>
#include <MailCommon/FolderTreeView>
#include <MailCommon/MailKernel>

#include <kitemmodels_version.h>

#include <QTreeView>
#include <QDebug>
#include <QEventLoop>
#include <QLabel>
#include <QStandardPaths>
#include <QTest>
#include <QTemporaryDir>

#include "dummykernel.cpp"

#define SHOW_WIDGET

using namespace Akonadi;

class FolderTreeWidgetTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        AkonadiTest::checkTestIsIsolated();

        DummyKernel *kernel = new DummyKernel(nullptr);
        CommonKernel->registerKernelIf(kernel);
        CommonKernel->registerSettingsIf(kernel);

        mFolderTreeWidget = new MailCommon::FolderTreeWidget(nullptr);
        const QStringList resourceOrder{"akonadi_knut_resource_2", "akonadi_knut_resource_0"}; // _1 isn't specified so it goes at the end
        mFolderTreeWidget->entityOrderProxy()->setTopLevelOrder(resourceOrder);

        mCollectionModel = KernelIf->collectionModel();
        mTopModel = mFolderTreeWidget->folderTreeView()->model();

        // One knut resource is already defined in the unittestenv, so that it's below "Search" in the ETM.
        QTRY_COMPARE(mCollectionModel->rowCount(), 2);
        QCOMPARE(mTopModel->rowCount(), 1); // Search doesn't appear yet
        mFolderNames = QStringList{"res1"};
        QCOMPARE(collectNames(mTopModel), mFolderNames);
    }

    void createAndDeleteSearchCollection()
    {
#if KITEMMODELS_VERSION <= QT_VERSION_CHECK(5, 25, 0)
        QSKIP("This test requires a fix in kitemmodels' KRecursiveFilterProxyModel");
#endif
#ifdef SHOW_WIDGET
        mFolderTreeWidget->resize(1000, 1000);
        mFolderTreeWidget->show();
#endif

        // Create search folder
        Akonadi::SearchQuery query;
        query.addTerm(Akonadi::SearchTerm(QStringLiteral("plugin"), 1));
        SearchCreateJob *create = new SearchCreateJob(QStringLiteral("search123456"), query, this);
        create->setSearchMimeTypes({KMime::Message::mimeType()});
        create->setRemoteSearchEnabled(false);
        AKVERIFYEXEC(create);
        const Collection searchCol = create->createdCollection();
        QVERIFY(searchCol.isValid());

        // Check it appeared in the tree, under a Search toplevel item
        QTRY_COMPARE(mCollectionModel->rowCount(), 2);
        QTRY_COMPARE(mTopModel->rowCount(), 2);
        const QStringList names = collectNames(mTopModel);
        QCOMPARE(names, QStringList() << "res1" << "Search");
        const int rowOfSearch = names.indexOf("Search");
        const QModelIndex searchParent = mTopModel->index(rowOfSearch, 0);
        QCOMPARE(mTopModel->rowCount(searchParent), 1); // the actual child search folder

#ifdef SHOW_WIDGET
        QTest::qWait(500);
#endif

        QSignalSpy rATBRSpy_CollectionModel(mCollectionModel, &QAbstractItemModel::rowsAboutToBeRemoved);
        QSignalSpy rATBRSpy_FolderTreeProxyModel(mFolderTreeWidget->folderTreeWidgetProxyModel(), &QAbstractItemModel::rowsAboutToBeRemoved);
        QSignalSpy rATBRSpy_TopModel(mTopModel, &QAbstractItemModel::rowsAboutToBeRemoved);

        // Now delete it
        Akonadi::CollectionDeleteJob *delJob = new Akonadi::CollectionDeleteJob(searchCol);
        AKVERIFYEXEC(delJob);

        // Check it disappeared from the tree, as well as the toplevel item
        QTRY_COMPARE(mTopModel->rowCount(), 1);
        QCOMPARE(collectNames(mTopModel), mFolderNames);
        QCOMPARE(collectNames(mFolderTreeWidget->entityOrderProxy()), mFolderNames);

        QCOMPARE(rATBRSpy_CollectionModel.count(), 2); // one for the child, one for the parent
        QCOMPARE(rATBRSpy_FolderTreeProxyModel.count(), 2); // one for the child, one for the parent
        QCOMPARE(rATBRSpy_TopModel.count(), 2); // one for the child, one for the parent

        checkMailFolders(mTopModel->index(0, 0));

#ifdef SHOW_WIDGET
        QTest::qWait(1000);
#endif
    }

    void testCreateResources()
    {
        // Test creating more knut resources.
        // This tests that ETM and proxies on top update correctly, and it tests toplevel collection order.
        const QVector<int> numFolders{ 1, 5, 2 };
        QVector<Collection> topLevelCollections;
        const AgentType agentType = AgentManager::self()->type(QStringLiteral("akonadi_knut_resource"));
        QVERIFY(agentType.isValid());
        mFolderNames = QStringList{"res3", "res1", "res2"}; // according to resourceOrder above (the folder names are defined in testdata-res*.xml)

        // Create resources
        const int numResources = numFolders.count();
        for (int i = 1 /*first one already created*/; i < numResources; ++i) {
            AgentInstanceCreateJob *agentCreateJob = new AgentInstanceCreateJob(agentType);
            AKVERIFYEXEC(agentCreateJob);
            const QString identifier = agentCreateJob->instance().identifier();

            QTRY_COMPARE(mCollectionModel->rowCount(), i + 1);
            QTRY_COMPARE(mTopModel->rowCount(), i + 1);

            Collection topLevelCollection;
            QTRY_VERIFY((topLevelCollection = topLevelCollectionForResource(identifier)).isValid());
            topLevelCollections.append(topLevelCollection);

            // Now create some folders
            for (int number = 0; number < numFolders[i]; ++number) {
                Collection mailCollection;
                mailCollection.setParentCollection(topLevelCollection);
                mailCollection.setName(QStringLiteral("mailCollection_%1_%2").arg(i).arg(number));
                CollectionCreateJob *collCreateJob = new CollectionCreateJob(mailCollection);
                AKVERIFYEXEC(collCreateJob);
            }
            const int resourceRow = collectNames(mTopModel).indexOf("res" + QString::number(i + 1));
            QModelIndex parent = mTopModel->index(resourceRow, 0);
            QTRY_COMPARE(mTopModel->rowCount(parent), numFolders[i]);

            checkMailFolders(parent);
        }
        QCOMPARE(collectNames(mTopModel), mFolderNames);
    }

    void testMoveFolder()
    {
#if QT_VERSION < QT_VERSION_CHECK(5, 8, 1) || KITEMMODELS_VERSION < QT_VERSION_CHECK(5, 30, 0)
        QSKIP("This test requires fixes in QSFPM, QIPM (which were made in Qt 5.8.1) and KExtraColumnsProxyModel >= 5.30");
#endif
        // Given a source folder with 2 levels of parents (res1/sub1/sub2)
        const QStringList folderNames = collectNames(mCollectionModel);
        const QModelIndex res1Index = mCollectionModel->index(folderNames.indexOf("res1"), 0, QModelIndex());
        const Collection topLevelCollection = res1Index.data(EntityTreeModel::CollectionRole).value<Collection>();
        QCOMPARE(topLevelCollection.name(), QStringLiteral("res1"));
        const int parentCount = 2;
        Collection currentColl = topLevelCollection;
        for (int number = 0; number < parentCount; ++number) {
            Collection mailCollection;
            mailCollection.setParentCollection(currentColl);
            mailCollection.setName(QStringLiteral("sub%1").arg(number+1));
            CollectionCreateJob *collCreateJob = new CollectionCreateJob(mailCollection);
            AKVERIFYEXEC(collCreateJob);
            currentColl = collCreateJob->collection();
        }
        QTRY_COMPARE(mCollectionModel->rowCount(res1Index), 1);

        // ... and a dest folder in another resource
        const QPersistentModelIndex res2Index = mCollectionModel->index(folderNames.indexOf("res2"), 0, QModelIndex());
        const int origRowCount = mCollectionModel->rowCount(res2Index);
        const Collection newParentCollection = res2Index.data(EntityTreeModel::CollectionRole).value<Collection>();
        QCOMPARE(newParentCollection.name(), QStringLiteral("res2"));

        QTest::qWait(100); // #### akonadi bug? Without this, a warning "Only resources can modify remote identifiers" appears

        // When moving the source folder (sub2) to the dest folder
        CollectionMoveJob *collMoveJob = new CollectionMoveJob(currentColl, newParentCollection);
        AKVERIFYEXEC(collMoveJob);

        // wait for Akonadi::Monitor::collectionMoved
        QTRY_COMPARE(mCollectionModel->rowCount(res2Index), origRowCount+1);

#ifdef SHOW_WIDGET
        QTest::qWait(1000);
#endif
    }

private:
    static Collection topLevelCollectionForResource(const QString &identifier)
    {
        // Find out the collection for the resource (as defined in unittestenv/xdglocal/testdata-res*.xml)
        CollectionFetchJob *job = new CollectionFetchJob(Collection::root(), CollectionFetchJob::FirstLevel);
        if (job->exec()) {
            const Collection::List collections = job->collections();
            for (const Collection &col : collections) {
                //qDebug() << col.resource() << col.mimeType();
                if (col.resource() == identifier) {
                    return col;
                }
            }
        } else {
            qWarning() << job->errorString();
        }
        return Collection();
    }

    // Check that every child of @p parent has a valid CollectionRole.
    static void checkMailFolders(const QModelIndex &parent)
    {
        const QAbstractItemModel *model = parent.model();
        QVERIFY(model);
        for (int row = 0; row < model->rowCount(parent); ++row) {
            QModelIndex idx = model->index(row, 0, parent);
            QModelIndex col1idx = model->index(row, 1, parent);
            QCOMPARE(col1idx.sibling(col1idx.row(), 0), idx);
            Collection collection = idx.data(EntityTreeModel::CollectionRole).value<Collection>();
            QVERIFY2(collection.isValid(), qPrintable(idx.data().toString()));
        }
    }

    static QStringList collectNames(QAbstractItemModel *model);
    EntityMimeTypeFilterModel *mCollectionModel;
    QAbstractItemModel *mTopModel;
    MailCommon::FolderTreeWidget *mFolderTreeWidget;
    QStringList mFolderNames;
};

QStringList FolderTreeWidgetTest::collectNames(QAbstractItemModel *model)
{
    QStringList ret;
    ret.reserve(model->rowCount());
    for (int row = 0; row < model->rowCount(); ++row) {
        ret.append(model->index(row, 0).data().toString());
    }
    return ret;
}

QTEST_AKONADIMAIN(FolderTreeWidgetTest)

#include "foldertreewidgettest.moc"
