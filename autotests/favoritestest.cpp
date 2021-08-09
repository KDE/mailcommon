/*
    SPDX-FileCopyrightText: 2013 Christian Mollekopf <mollekopf@kolabsys.com>
    SPDX-FileCopyrightText: 2017 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QObject>

#include <KConfigGroup>
#include <KSharedConfig>

#include <changerecorder.h>
#include <control.h>
#include <entitytreemodel.h>
#include <favoritecollectionorderproxymodel.h>
#include <favoritecollectionsmodel.h>
#include <qtest_akonadi.h>

using namespace Akonadi;
using namespace MailCommon;

class FavoriteProxyTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testReordering();

private:
    EntityTreeModel *createETM();
};

void FavoriteProxyTest::initTestCase()
{
    AkonadiTest::checkTestIsIsolated();
    Akonadi::Control::start();
    AkonadiTest::setAllResourcesOffline();
}

static QModelIndex getIndex(const QString &string, EntityTreeModel *model)
{
    QModelIndexList list = model->match(model->index(0, 0), Qt::DisplayRole, string, 1, Qt::MatchRecursive);
    if (list.isEmpty()) {
        return QModelIndex();
    }
    return list.first();
}

/**
 * Since we have no sensible way to figure out if the model is fully populated,
 * we use the brute force approach.
 */
static bool waitForPopulation(const QModelIndex &idx, EntityTreeModel *model, int count)
{
    for (int i = 0; i < 500; i++) {
        if (model->rowCount(idx) >= count) {
            return true;
        }
        QTest::qWait(10);
    }
    return false;
}

EntityTreeModel *FavoriteProxyTest::createETM()
{
    auto changeRecorder = new ChangeRecorder(this);
    changeRecorder->setCollectionMonitored(Collection::root());
    auto model = new EntityTreeModel(changeRecorder, this);
    model->setItemPopulationStrategy(Akonadi::EntityTreeModel::LazyPopulation);
    return model;
}

static const int numberOfRootCollections = 4;

void FavoriteProxyTest::testReordering()
{
    // GIVEN an ETM, a favorite proxy, and an EntityOrderProxyModel on top
    EntityTreeModel *model = createETM();
    QVERIFY(waitForPopulation(QModelIndex(), model, numberOfRootCollections));

    QList<Collection::Id> collectionIds;
    QStringList labels;
    QStringList order;
    for (const QString &folderName : {QStringLiteral("res2"), QStringLiteral("res3")}) {
        const QModelIndex index = getIndex(folderName, model);
        QVERIFY(index.isValid());
        const auto favoriteCollection = index.data(EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        QVERIFY(favoriteCollection.isValid());
        collectionIds.push_back(favoriteCollection.id());
        order.push_back(QLatin1Char('c') + QString::number(favoriteCollection.id()));
        labels << folderName;
    }

    KConfigGroup configGroup(KSharedConfig::openConfig(), "favoritecollectionsmodeltest");
    configGroup.writeEntry("FavoriteCollectionIds", collectionIds);
    configGroup.writeEntry("FavoriteCollectionLabels", labels);
    configGroup.writeEntry("0", order);

    auto favoriteModel = new FavoriteCollectionsModel(model, configGroup, this);
    QTRY_COMPARE(favoriteModel->rowCount(), 2);

    auto orderProxy = new FavoriteCollectionOrderProxyModel(this);
    orderProxy->setOrderConfig(configGroup);
    orderProxy->setSourceModel(favoriteModel);
    orderProxy->sort(0, Qt::AscendingOrder);

    QCOMPARE(orderProxy->rowCount(), 2);

    const QModelIndex firstRowIndex = orderProxy->index(0, 0);
    QVERIFY(firstRowIndex.isValid());
    QCOMPARE(firstRowIndex.data().toString(), QStringLiteral("res2"));
    // we can drop emails
    QVERIFY((orderProxy->flags(firstRowIndex) & Qt::ItemIsDropEnabled) != 0);
}

#include "favoritestest.moc"

QTEST_AKONADIMAIN(FavoriteProxyTest)
