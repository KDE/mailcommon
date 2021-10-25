/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <Akonadi/Collection>
#include <QObject>
class KJob;
namespace MailCommon
{
class CollectionExpiryJob : public QObject
{
    Q_OBJECT
public:
    explicit CollectionExpiryJob(QObject *parent = nullptr);
    ~CollectionExpiryJob() override;

    void start();

    void setCollection(const Akonadi::Collection &collection);

    void setExpireNow(bool expireNow);

private:
    void slotCollectionModified(KJob *job);
    bool mExpireNow = false;
    Akonadi::Collection mCollection;
};
}

