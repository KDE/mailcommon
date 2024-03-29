/*
  SPDX-FileCopyrightText: 2016 David Faure <faure@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#undef QT_NO_CAST_FROM_ASCII

#include <akonadi/qtest_akonadi.h>

#include <Akonadi/Collection>
#include <Akonadi/Item>
#include <Akonadi/ItemFetchScope>

#include <Akonadi/AgentInstanceCreateJob>
#include <Akonadi/AgentManager>
#include <Akonadi/AgentType>
#include <Akonadi/CollectionCreateJob>
#include <Akonadi/CollectionDeleteJob>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionMoveJob>
#include <Akonadi/EntityOrderProxyModel>
#include <Akonadi/SearchCreateJob>
#include <Akonadi/SearchQuery>

#include <KMime/Message>

#include "folder/entitycollectionorderproxymodel.h"
#include <MailCommon/FolderTreeView>
#include <MailCommon/FolderTreeWidget>
#include <MailCommon/MailKernel>

#include <QDebug>
#include <QStandardPaths>
#include <QTest>
#include <QTreeView>

#include "dummykernel.cpp"

// #define SHOW_WIDGET

using namespace Akonadi;

class FolderTreeWidgetTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        AkonadiTest::checkTestIsIsolated();

        auto kernel = new DummyKernel(nullptr);
        CommonKernel->registerKernelIf(kernel);
        CommonKernel->registerSettingsIf(kernel);

        mFolderTreeWidget = new MailCommon::FolderTreeWidget(nullptr);
        const QStringList resourceOrder{"akonadi_knut_resource_2", "akonadi_knut_resource_0"}; // _1 isn't specified so it goes at the end
        mFolderTreeWidget->entityOrderProxy()->setTopLevelOrder(resourceOrder);

        mCollectionModel = KernelIf->collectionModel();
        mTopModel = mFolderTreeWidget->folderTreeView()->model();

        // One knut resource is already defined in the unittestenv, so that it's below "Search" in the ETM.
        QTRY_COMPARE(mCollectionModel->rowCount(), 4);
        QCOMPARE(mTopModel->rowCount(), 3); // Search doesn't appear yet
        mFolderNames = QStringList{QStringLiteral("res3"), QStringLiteral("res1"), QStringLiteral("res2")};
        QCOMPARE(collectNames(mTopModel), mFolderNames);
    }

    void createAndDeleteSearchCollection()
    {
#ifdef SHOW_WIDGET
        mFolderTreeWidget->resize(1000, 1000);
        mFolderTreeWidget->show();
#endif

        // Create search folder
        Akonadi::SearchQuery query;
        query.addTerm(Akonadi::SearchTerm(QStringLiteral("plugin"), 1));
        auto create = new SearchCreateJob(QStringLiteral("search123456"), query, this);
        create->setSearchMimeTypes({KMime::Message::mimeType()});
        create->setRemoteSearchEnabled(false);
        AKVERIFYEXEC(create);
        const Collection searchCol = create->createdCollection();
        QVERIFY(searchCol.isValid());

        // Check it appeared in the tree, under a Search toplevel item
        QTRY_COMPARE(mCollectionModel->rowCount(), 4);
        QTRY_COMPARE(mTopModel->rowCount(), 4);
        const QStringList names = collectNames(mTopModel);
        const QStringList expectedNames{"res3", "res1", "res2", "Search"};
        QCOMPARE(names, expectedNames);
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
        auto delJob = new Akonadi::CollectionDeleteJob(searchCol);
        AKVERIFYEXEC(delJob);

        // Check it disappeared from the tree, as well as the toplevel item
        QTRY_COMPARE(mTopModel->rowCount(), 3);
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
        const QList<int> numFolders{1, 5, 2};
        QList<Collection> topLevelCollections;
        const AgentType agentType = AgentManager::self()->type(QStringLiteral("akonadi_knut_resource"));
        QVERIFY(agentType.isValid());
        mFolderNames = QStringList{"res3", "res1", "res2"}; // according to resourceOrder above (the folder names are defined in testdata-res*.xml)

        // Create resources
        const int numResources = numFolders.count();
        for (int i = 3 /*first three already created*/; i < numResources; ++i) {
            auto agentCreateJob = new AgentInstanceCreateJob(agentType);
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
                auto collCreateJob = new CollectionCreateJob(mailCollection);
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
        // Given a source folder with 2 levels of parents (res1/sub1/sub2)
        const QStringList folderNames = collectNames(mCollectionModel);
        const QModelIndex res1Index = mCollectionModel->index(folderNames.indexOf("res1"), 0, QModelIndex());
        const auto topLevelCollection = res1Index.data(EntityTreeModel::CollectionRole).value<Collection>();
        QCOMPARE(topLevelCollection.name(), QStringLiteral("res1"));
        const int parentCount = 2;
        Collection currentColl = topLevelCollection;
        for (int number = 0; number < parentCount; ++number) {
            Collection mailCollection;
            mailCollection.setParentCollection(currentColl);
            mailCollection.setName(QStringLiteral("sub%1").arg(number + 1));
            auto collCreateJob = new CollectionCreateJob(mailCollection);
            AKVERIFYEXEC(collCreateJob);
            currentColl = collCreateJob->collection();
        }
        QTRY_COMPARE(mCollectionModel->rowCount(res1Index), 1);

        // ... and a dest folder in another resource
        const QPersistentModelIndex res2Index = mCollectionModel->index(folderNames.indexOf("res2"), 0, QModelIndex());
        const int origRowCount = mCollectionModel->rowCount(res2Index);
        const auto newParentCollection = res2Index.data(EntityTreeModel::CollectionRole).value<Collection>();
        QCOMPARE(newParentCollection.name(), QStringLiteral("res2"));

        QTest::qWait(100); // #### akonadi bug? Without this, a warning "Only resources can modify remote identifiers" appears

        // When moving the source folder (sub2) to the dest folder
        auto collMoveJob = new CollectionMoveJob(currentColl, newParentCollection);
        AKVERIFYEXEC(collMoveJob);

        // wait for Akonadi::Monitor::collectionMoved
        QTRY_COMPARE(mCollectionModel->rowCount(res2Index), origRowCount + 1);

#ifdef SHOW_WIDGET
        QTest::qWait(1000);
#endif
    }

    void testFiltering()
    {
        const auto model = mFolderTreeWidget->entityOrderProxy();
        QCOMPARE(collectNamesRecursive(model), (QStringList{"res3", "res1", "sub1", "res2", "sub2"}));

        mFolderTreeWidget->applyFilter(QStringLiteral("sub"));
        // matches all folders matching "sub"
        QCOMPARE(collectNamesRecursive(model), (QStringList{"res1", "sub1", "res2", "sub2"}));
        QCOMPARE(mFolderTreeWidget->currentIndex().data().toString(), "sub1");

        mFolderTreeWidget->applyFilter(QStringLiteral("res"));
        // matches all folders matching "res"
        QCOMPARE(collectNamesRecursive(model), (QStringList{"res3", "res1", "res2"}));
        // "res1" is current because it became current when previous current "sub1" was filtered out
        QCOMPARE(mFolderTreeWidget->currentIndex().data().toString(), "res1");

        mFolderTreeWidget->applyFilter(QStringLiteral("foo"));
        // matches nothing
        QCOMPARE(collectNamesRecursive(model), (QStringList{}));
        QVERIFY(!mFolderTreeWidget->currentIndex().isValid());

        mFolderTreeWidget->applyFilter(QStringLiteral("res/sub"));
        // matches folders matching "sub" with parents matching "res"
        QCOMPARE(collectNamesRecursive(model), (QStringList{"res1", "sub1", "res2", "sub2"}));
        QCOMPARE(mFolderTreeWidget->currentIndex().data().toString(), "sub1");

        mFolderTreeWidget->applyFilter(QStringLiteral("res/1"));
        // matches folders matching "1" with parents matching "res"
        QCOMPARE(collectNamesRecursive(model), (QStringList{"res1", "sub1"}));
        QCOMPARE(mFolderTreeWidget->currentIndex().data().toString(), "sub1");

        mFolderTreeWidget->applyFilter(QStringLiteral("res/"));
        // matches folders matching anything ("" always matches) with parents matching "res"
        QCOMPARE(collectNamesRecursive(model), (QStringList{"res1", "sub1", "res2", "sub2"}));
        QCOMPARE(mFolderTreeWidget->currentIndex().data().toString(), "sub1");

        mFolderTreeWidget->applyFilter(QStringLiteral("sub/"));
        // matches nothing (there are no folders matching "sub" that have subfolders)
        QCOMPARE(collectNamesRecursive(model), (QStringList{}));
        QVERIFY(!mFolderTreeWidget->currentIndex().isValid());

        mFolderTreeWidget->applyFilter(QStringLiteral("/sub"));
        // matches folders matching "sub" with parents matching anything
        QCOMPARE(collectNamesRecursive(model), (QStringList{"res1", "sub1", "res2", "sub2"}));
        QCOMPARE(mFolderTreeWidget->currentIndex().data().toString(), "sub1");

        mFolderTreeWidget->applyFilter(QStringLiteral("/res"));
        // matches nothing (there are no subfolders matching "res")
        QCOMPARE(collectNamesRecursive(model), (QStringList{}));
        QVERIFY(!mFolderTreeWidget->currentIndex().isValid());

        mFolderTreeWidget->applyFilter(QStringLiteral("//sub"));
        // matches nothing (there are no subsubfolders matching "sub")
        QCOMPARE(collectNamesRecursive(model), (QStringList{}));
        QVERIFY(!mFolderTreeWidget->currentIndex().isValid());

        mFolderTreeWidget->applyFilter(QStringLiteral("res//"));
        // matches nothing (there are no folders matching "res" that have subsubfolders)
        QCOMPARE(collectNamesRecursive(model), (QStringList{}));
        QVERIFY(!mFolderTreeWidget->currentIndex().isValid());

        mFolderTreeWidget->applyFilter(QStringLiteral("1/1"));
        // matches folders matching "1" with parents matching "1"
        QCOMPARE(collectNamesRecursive(model), (QStringList{"res1", "sub1"}));
        QCOMPARE(mFolderTreeWidget->currentIndex().data().toString(), "sub1");

        mFolderTreeWidget->applyFilter(QStringLiteral("2/"));
        // matches folders matching anything with parents matching "2"
        QCOMPARE(collectNamesRecursive(model), (QStringList{"res2", "sub2"}));
        QCOMPARE(mFolderTreeWidget->currentIndex().data().toString(), "sub2");

        mFolderTreeWidget->applyFilter(QStringLiteral("1/2"));
        // matches nothing (there are no folders matching "2" with parents matching "1")
        QCOMPARE(collectNamesRecursive(model), (QStringList{}));
        QVERIFY(!mFolderTreeWidget->currentIndex().isValid());
    }

private:
    static Collection topLevelCollectionForResource(const QString &identifier)
    {
        // Find out the collection for the resource (as defined in unittestenv/xdglocal/testdata-res*.xml)
        auto job = new CollectionFetchJob(Collection::root(), CollectionFetchJob::FirstLevel);
        if (job->exec()) {
            const Collection::List collections = job->collections();
            for (const Collection &col : collections) {
                // qDebug() << col.resource() << col.mimeType();
                if (col.resource() == identifier) {
                    return col;
                }
            }
        } else {
            qWarning() << job->errorString();
        }
        return {};
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
            auto collection = idx.data(EntityTreeModel::CollectionRole).value<Collection>();
            QVERIFY2(collection.isValid(), qPrintable(idx.data().toString()));
        }
    }

    static QStringList collectNamesRecursive(const QAbstractItemModel *model, const QModelIndex &parent = QModelIndex{})
    {
        QStringList ret;
        ret.reserve(model->rowCount(parent));
        for (int row = 0; row < model->rowCount(parent); ++row) {
            QModelIndex idx = model->index(row, 0, parent);
            ret.append(idx.data().toString());
            ret.append(collectNamesRecursive(model, idx));
        }
        return ret;
    }

    static QStringList collectNames(QAbstractItemModel *model);
    EntityMimeTypeFilterModel *mCollectionModel = nullptr;
    QAbstractItemModel *mTopModel = nullptr;
    MailCommon::FolderTreeWidget *mFolderTreeWidget = nullptr;
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
