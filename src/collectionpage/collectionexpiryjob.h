/*
  SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COLLECTIONEXPIRYJOB_H
#define COLLECTIONEXPIRYJOB_H

#include <QObject>
#include <AkonadiCore/Collection>
class KJob;
namespace MailCommon {
class CollectionExpiryJob : public QObject
{
    Q_OBJECT
public:
    explicit CollectionExpiryJob(QObject *parent = nullptr);
    ~CollectionExpiryJob();

    void start();

    void setCollection(const Akonadi::Collection &collection);

    void setExpireNow(bool expireNow);

private:
    void slotCollectionModified(KJob *job);
    bool mExpireNow = false;
    Akonadi::Collection mCollection;
};
}

#endif // COLLECTIONEXPIRYJOB_H
